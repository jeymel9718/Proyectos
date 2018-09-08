#include "cache.h"

#include <iostream>

Cache::Cache(int* clk_,int* data_,int* addr_,bool* read_,bool* write_,bool* mem_read_,bool* mem_write_,MainWindow* win){
  this->clk=clk_;
  this->data=data_;
  this->cpu_read=read_;
  this->cpu_write=write_;
  this->addr=addr_;
  this->mw=win;
  this->mem_write=mem_write_;
  this->mem_read=mem_read_;
}

void Cache::run(){
  while(true){
      if(*clk && !clk_aux){
          clk_aux=1;

          if(*cpu_read && !waiting) {
              if(mem_s[*addr] == 'i'){ //bloque invalido, necesario traerlo de memoria
                  waiting=1;
                  *mem_read=1;
              }else {
                  *data=mem[*addr];
                  *cpu_read=0;
              }
          } else if(*cpu_write && !waiting) { //escritura
              waiting=1;
              *mem_write=1;
          }

          if(!*mem_write && waiting && *cpu_write){ //termino la escritura
              waiting=0;
              mem_s[*addr]='p';
              mem[*addr]=*data;
              mw->UpdateView(numero,*addr,QString::number(*addr)+" | p | "+QString::number(*data));
              *cpu_write=0;
          }else if(!*mem_read && waiting && *cpu_read){ //termino la lectura
              waiting=0;
              mem_s[*addr]='s';
              mem[*addr]=*data;
              mw->UpdateView(numero,*addr,QString::number(*addr)+" | s | "+QString::number(*data));
              *cpu_read=0;
          }
      }else if(!*clk && clk_aux){
          clk_aux=0;
      }
    std::this_thread::sleep_for (std::chrono::seconds(1));
  }
}

std::thread Cache::trun(){
  return std::thread([=]{run();});
}

void Cache::setNumber(int number_){
    this->numero=number_;
}

/*
 * Invalida el bloque de cache indicado
 * @param dir, direccion del bloque
*/
void Cache::updateCachei(int dirr){
    mem_s[dirr]='i';
    mw->UpdateView(numero,dirr,QString::number(dirr)+" | i | "+QString::number(mem[dirr]));
}

/*
 * Actauliza el bloque de cache de privado a compartido
 * @param dir, direccion del bloque
*/
void Cache::updateCaches(int dirr){
    if(mem_s[dirr]=='p'){
        mem_s[dirr]='s';
        mw->UpdateView(numero,dirr,QString::number(dirr)+" | s | "+QString::number(mem[dirr]));
    }
}
