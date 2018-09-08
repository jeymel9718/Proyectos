#ifndef CONTROLCACHE_H
#define CONTROLCACHE_H

#include <thread>
#include <mutex>
#include "cache.h"
class ControlCache
{
public:
    ControlCache(int*,int*,int*,int*,int*,bool*,bool*,bool*,bool*,bool*,bool*,Cache*);
    void run();
    std::thread trun();
private:
    //bus del cache
    bool *cache_read;
    bool *cache_write;
    int* cache_data;
    int* addr;

    //señales de control conectadas a la cola
    bool *tickect;
    bool *ready;

    /* Bus compartido */
    bool *mem_read;
    bool *mem_write;
    int* mem_addr;
    int* mem_data;

    int *clk;
    bool clk_aux=0;
    bool waiting=0;
    bool wait_mem=0;
    Cache* cache;
};

#endif // CONTROLCACHE_H
