#include <QCoreApplication>
#include <iostream>
#include <string>
#include "grafopdf.h"
#include"Dictionary.h"
#include <time.h>
#include <math.h>
#include<fstream>
#include<thread>
#include <stdio.h>
#include <clocale>
#include <cmath>
#include "stemming/spanish_stem.h"
#include <cmath>

#include<stdlib.h>



using namespace std;

///stringiable global que guardara los resultados de las comparaciones
//std::map<string,map<string,vector<double>>> resultados_finales;
std::map<string,map<string,double>> resultados_finales;


int re=0;

void copiar_textos_a_ficheros()
{
    ifstream file("bd2.csv");
    ofstream f("bd/files.txt",ios::app);

    string a,b,c;
    int i=0;

    while(!file.eof())
    {
        getline(file,a,'@');
        getline(file,b,'@');
        getline(file,c,'&');
       if((a.length()!=0 && (a>"100")) && (b.length()!=0) && (c.length()!=0) )
       {
           ofstream bd("bd/"+a+".txt");
           i++;
            bd<<c;
            f<<a+".txt"<<endl;


       }

//4915

    }
}


void remplazar1 (string &letra)
{
      char con[] = {'Ã','á','à','ä','é','è','ë','í','ì','ï','ó','ò','ö','ú','ù','u','ñ','Á','À','Ä','É','È','Ë','Í','Ì','Ï','Ó','Ò','Ö','Ú','Ù','Ü','Ñ','ç','Ç'};
      string sin[] ={ "a","a","a","a","e","e","e","i","i","i","o","o","o","u","u","u","n","A","A","A","E","E","E","I","I","I","O","O","O","U","U","U","N","c","C"};
      int tam=letra.length();
     cout<<tam;
     for(int j=0;j<tam;j++)
     {
          bool flag=false;

          for (int i = 0; i <35; i++)
          {
              if(letra[j]==con[i])
              {


                  letra.erase(j-1,1);
                letra.replace(j-1,1,sin[i]);


              }
          }

     }


}



void probar_experimentos()
{
    clock_t t;
   ///  t = clock();
     Dictionary dictionary,p;
     int adyacencia=2;
     int keywords=10;
     double rho=3;
     int num=1600;      ///numero de documentos que probaremos

    ///Cargo los texto y convierto a grafos para poder hacer las comparaciones luego
    Grafopdf *lista_grafos[num];int a=0;
    for(int i=1;i<=num;i++)
    {
        lista_grafos[i-1]=new  Grafopdf(to_string(i),adyacencia,dictionary);
        a++;     //lista_grafos[i-1]->draw_dot();
    }
    ///hago las comparaciones finales de los textos paralelamente
    t = clock();

    cout<<"loading comparaciones"<<endl;
    for(int i=0;i<(num-1);i++)
    {
      for(int j=i+1;j<num;j++)
      {
           double result=lista_grafos[i]->compare(lista_grafos[j],keywords,rho);
           resultados_finales[lista_grafos[i]->file][lista_grafos[j]->file]= result;
           re++;      cout<<re<<" ";
      }
    }

  t = clock() - t;


  std::ostringstream oss; oss << rho;string Rho=oss.str();

     ofstream fileres("resultados_amazon/resultados_amazon_prueba1_kw"+to_string(keywords)+"_rho"+Rho+"_ayd"+to_string(adyacencia)+".txt",ios::app);
    for(auto &it1:resultados_finales)
    {
        for(auto &it2:it1.second)
        {
              fileres<<it1.first<<" "<<it2.first<<" "<<it2.second<<endl;
        }
    }
     cout<<"Time: " << ((float)t/CLOCKS_PER_SEC) <<" sec "<<endl ;
}


void get_vector_caracteristica()
{
    string categoria="toys_&_games";
    string path="resultados_journal/resultados_amazon_"+categoria;

    ifstream leer(path+"/vectores.txt");
    map<string,int> VC;
    while(!leer.eof())
    {
        string word;
        leer>>word;
        VC[word]++;
    }
    ofstream escribir(path+"/vector_Caracteristica.txt");
    std::multimap<int,string> mymultimap;
    for(auto it:VC)
    {
        if(it.second>3000)
            mymultimap.insert (std::pair<int,string>(it.second,it.first));

    }
    for (std::multimap<int,string>::iterator it=mymultimap.begin(); it!=mymultimap.end(); ++it)
      escribir<< (*it).first << " " << (*it).second <<endl;

}



///funcion para convertir y completar con ceros los vectores caracteristicas para el entremiento
void convertir_vectores_caracteristicas_los_resultados()
{
    ifstream leer1("resultados_journal_prueba_2/resultados_amazon_software/vectores_2.txt");
    ifstream leer2("resultados_journal_prueba_2/resultados_amazon_dvd/vectores_2.txt");
    ifstream leer3("resultados_journal_prueba_2/resultados_amazon_baby/vectores_2.txt");
    ifstream leer4("resultados_journal_prueba_2/resultados_amazon_toys_&_games/vectores_2.txt");
    map<string,int> word;
    map<string,int> word_id;

    ///obtener todas las palabras para crear un diccionario
    while(!leer1.eof())  { string palabra; leer1>>palabra;  word[palabra]++;}
    while(!leer2.eof())  { string palabra; leer2>>palabra;  word[palabra]++;}
    while(!leer3.eof())  { string palabra; leer3>>palabra;  word[palabra]++;}
    while(!leer4.eof())  { string palabra; leer4>>palabra;  word[palabra]++;}


    ////asignos creo un id para cada palabra de los vectores caracteristica ////  ignore
    int id=0;
    for(auto it:word){word_id[it.first]=id; id++;}



    ////cambio de palabra a id y creo los vectores esparsos para el posterior entrenamiento
    string line;
   int a=0;

   string categoria="toys_&_games";//baby dvd software
   ofstream escribir("resultados_journal_prueba_2/resultados_amazon_"+categoria+"/vectores_finales.txt");
   ifstream leer("resultados_journal_prueba_2/resultados_amazon_"+categoria+"/vectores_2.txt");
   while(!leer.eof())
    {
      getline(leer,line);
      stringstream list_str(line);
      string word;int i=0;
      map<string,int> vector;
      while(list_str >> word)
      {
         i++;
        // cout<<word<<endl;
         vector[word]=1;

      }

     /////escribir los vectores finales
      for(auto it:word_id)
      {
          escribir<<vector[it.first]<<" ";
      }escribir<<endl;
    }


   //software >50  233
   //baby  >30   267
   //dvd > 30    328
   //toys_&_games >20 155
   ofstream guardar_salidas("resultados_journal_prueba_2/vectores_caracteristica/salida.txt",ios::app);
   for(int i=0;i<155;i++)
   {
      /// guardar_salidas<<4<<" ";/// solo usar para llenar las salidas deseadas
   }




}

////funcion para seleccionar los vectores con los q trabaje posteriormetne en la red neuronal
void seleccionar_Vectores_caracteristica()
{

    //software >50  233
    //baby  >30   267
    //dvd > 30    328
    //toys_&_games >20 155
      ifstream leer1("resultados_journal_prueba_2/resultados_amazon_toys_&_games/vectores.txt");
      ofstream escribir("resultados_journal_prueba_2/resultados_amazon_toys_&_games/vectores_2.txt");
      string line;
     int a=0;
      while(!leer1.eof())
      {
        getline(leer1,line);
        stringstream list_str(line);
        string word;int i=0;
        while(list_str >> word)
        {
           i++;
           //cout<<word<<endl;

        }
        if(i>15){
            escribir<<line<<endl;a++;
        }
      }
      cout<<a<<endl;
}



int main()
{


     probar_experimentos(); ///hacer las comparaciones todos contra todos
     get_vector_caracteristica(); /// a partir de los resultados de las comparaciones obtener vector caract. de la categoria
     get_resultados_finales();

    convertir_vectores_caracteristicas_los_resultados();
    seleccionar_Vectores_caracteristica();
 

}
