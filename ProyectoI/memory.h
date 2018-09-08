#ifndef MEMORY_H
#define MEMORY_H

#include <thread>
#include <mutex>
#include "mainwindow.h"

class Memory
{
public:
    Memory(int*,int*,int*,bool*,bool*,MainWindow*);
    void run();
    std::thread trun();

private:
    int mem[16]={}; //memoria compartida
    int* clk;
    /* Bus compartido */
    bool *mem_read;
    bool *mem_write;
    int *mem_addr;
    int *mem_data;

    MainWindow *mw;
    bool latency,clk_aux = 0;
};

#endif // MEMORY_H
