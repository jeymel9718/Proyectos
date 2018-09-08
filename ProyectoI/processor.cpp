#include "processor.h"

#include <iostream>
#include <cstdlib>
#include <chrono>

enum _States {Procesando = 0, Escritura = 1, Lectura = 2};
extern int clk;
extern std::mutex mtx;

bool condition(){
    return clk!=0;
}

Processor::Processor(int* data_,int* addr_,bool* write_,bool* read_,MainWindow *w){
  this->mw=w;
    this->c_read=read_;
    this->c_write=write_;
    this->data=data_;
    this->addr=addr_;
}

void Processor::run(){

  while(true){
      if(clk && !r){
          State = rand() % 3;
          r=1;
          if(*c_write==0 && *c_read==0){ //se termino la accion anterior
              if(imprimir){ //imprimir la lectura en GUI
                  imprimir=0;
                  mw->UpdateView(number,0,"Lectura exitosa: "+QString::number(*data));
                  std::this_thread::sleep_for (std::chrono::seconds(2));

              }else if(ips){ //Exito de escritura
                  ips=0;
                  mw->UpdateView(number,0,"Escritura exitosa");
                  std::this_thread::sleep_for (std::chrono::seconds(2));
              }

              switch (State) {
                case Procesando:
                  mw->UpdateView(number,9,"Procesamiento");
                  break;
                case Escritura:
                  *data=rand() % 100;
                  *addr=rand() % 16;
                  mw->UpdateView(number,0,"Escritura en: "+QString::number(*addr)+"\n "+QString::number(*data));
                  *c_write=1;
                  ips=1;
                  break;
                case Lectura:
                  imprimir=1;
                  *addr=rand() % 16;
                  mw->UpdateView(number,0,"Lectura en: "+QString::number(*addr));
                  *c_read=1;
                  break;
              }
          }
      }else if(!clk && r){
          r=0;
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

std::thread Processor::trun(){
  return std::thread([=]{run();});
}

void Processor::setNumber(int number_){
    this->number=number_;
}
