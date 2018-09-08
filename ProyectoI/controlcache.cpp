#include "controlcache.h"

#include <mutex>
#include <iostream>


ControlCache::ControlCache(int* clk_,int* mem_addr_,int* mem_data_,int* data_,int* addr_,
                           bool* tickect_,bool *mem_read_,bool *mem_write_,bool* read_,bool *write_,bool* ready_,Cache* c)
{
    this->clk=clk_;
    this->mem_addr=mem_addr_;
    this->mem_data=mem_data_;
    this->addr=addr_;
    this->cache_data=data_;
    this->cache_read=read_;
    this->cache_write=write_;
    this->mem_read=mem_read_;
    this->mem_write=mem_write_;
    this->tickect=tickect_;
    this->ready=ready_;
    this->cache=c;
}

void ControlCache::run(){
    while(true){
        if(*clk==1 && !clk_aux){
            clk_aux=1;
            if((*cache_read || *cache_write) && !*tickect && !waiting){ //se necesita ir a memoria
                waiting=1;
                *tickect=1;
            }

            /* El bus se encuentra libre */
            if(waiting && !*tickect && *cache_read){
                waiting=0;
                wait_mem=1; //se debe esperar a que la memoria termine
                *mem_addr=*addr;
                *mem_read=1;
            }else if(waiting && !*tickect && *cache_write){
                waiting=0;
                wait_mem=1;
                *mem_addr=*addr;
                *mem_data=*cache_data;
                *mem_write=1;
            }

            if(!*mem_read && wait_mem && *cache_read){ //termino la lectura
                *cache_data=*mem_data;
                *cache_read=0;
                *ready=0;
                wait_mem=0;
            }else if(!*mem_write && wait_mem && *cache_write){ //termino la escritura
                *cache_write=0;
                *ready=0;
                wait_mem=0;
            }

        }else if(*clk==0 && clk_aux){
            clk_aux=!clk_aux;
            if(*mem_read && !wait_mem){ //transaccion de lectura en el bus de otra cache
                cache->updateCaches(*mem_addr);
            }else if(*mem_write && !wait_mem){ //transaccion de escritura en el bus de otra cache
                cache->updateCachei(*mem_addr);
            }
        }
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
}

std::thread ControlCache::trun(){
  return std::thread([=]{run();});
}
