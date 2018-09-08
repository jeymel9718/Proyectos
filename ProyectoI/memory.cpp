#include "memory.h"

#include <iostream>

Memory::Memory(int* clk_,int *addr_,int* data_,bool* read_,bool* write_,MainWindow* w)
{
    this->clk=clk_;
    this->mem_addr=addr_;
    this->mem_data=data_;
    this->mem_read=read_;
    this->mem_write=write_;
    this->mw=w;
}

void Memory::run(){
    while(true){
        if(*clk == 1 && !clk_aux && !latency){ //simula la latencia de acceder a memoria
            clk_aux=1;
            mw->addClk();
            if(*mem_read || *mem_write){
                latency=1;
            }
        }else if(*clk==1 && !clk_aux && latency){
            clk_aux=1;
            mw->addClk();
            if(*mem_read){ //lectura
                *mem_data=mem[*mem_addr];
                *mem_read=0;
                latency=0;
            }else if(*mem_write){ //escritura
                mem[*mem_addr]=*mem_data;
                mw->UpdateView(9,*mem_addr,QString::number(*mem_addr)+" | "+QString::number(*mem_data));
                *mem_write=0;
                latency=0;
            }
        }else if(*clk==0 && clk_aux){
            clk_aux=!clk_aux;
        }
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
}

std::thread Memory::trun(){
  return std::thread([=]{run();});
}
