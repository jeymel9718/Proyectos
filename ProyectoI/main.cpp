#include "mainwindow.h"
#include "processor.h"
#include "cache.h"
#include "controlcache.h"
#include "cola.h"
#include "memory.h"
#include <QApplication>
#include <thread>
#include <iostream>
#include <chrono>

int clk=0;
std::mutex mtx;
std::condition_variable cv;

//signals cpu1 and cache1
bool cpu_read,cpu_write,busy,cmem,cmemw = 0;
int cdatas,caddr = 0;

//signals cpu2 and cache2
bool cpu_read2,cpu_write2,tt2,cmem2,cmemw2 = 0;
int cdatas2,caddr2 =0;
bool tt3,tt4=0;

//signals cpu3 and cache3
bool cpu_read3,cpu_write3,cmem3,cmemw3 = 0;
int cdatas3,caddr3 =0;

//signals cpu4 and cache4
bool cpu_read4,cpu_write4,cmem4,cmemw4 = 0;
int cdatas4,caddr4 =0;

bool task_ready=0;

/* Bus */
int mem_data = 0;
int mem_addr=0;
bool mem_read=0;
bool mem_write=0;

/*
 * Simualte clk signal
*/
void clk_signal(){
  while(true) {
    clk=!clk;
    std::this_thread::sleep_for (std::chrono::seconds(5));
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //ventana del programa
    MainWindow w;
    /*Declaracion de cada unidad de harware*/
    Processor* CPU1=new Processor(&cdatas,&caddr,&cpu_write,&cpu_read,&w);
    Cache* c1=new Cache(&clk,&cdatas,&caddr,&cpu_read,&cpu_write,&cmem,&cmemw,&w);
    ControlCache* cc1=new ControlCache(&clk,&mem_addr,&mem_data,&cdatas,&caddr,&busy,
                                       &mem_read,&mem_write,&cmem,&cmemw,&task_ready,c1);

    Processor* CPU2=new Processor(&cdatas2,&caddr2,&cpu_write2,&cpu_read2,&w);
    Cache* c2=new Cache(&clk,&cdatas2,&caddr2,&cpu_read2,&cpu_write2,&cmem2,&cmemw2,&w);
    ControlCache* cc2=new ControlCache(&clk,&mem_addr,&mem_data,&cdatas2,&caddr2,&tt2,
                                       &mem_read,&mem_write,&cmem2,&cmemw2,&task_ready,c2);

    Processor* CPU3=new Processor(&cdatas3,&caddr3,&cpu_write3,&cpu_read3,&w);
    Cache* c3=new Cache(&clk,&cdatas3,&caddr3,&cpu_read3,&cpu_write3,&cmem3,&cmemw3,&w);
    ControlCache* cc3=new ControlCache(&clk,&mem_addr,&mem_data,&cdatas3,&caddr3,&tt3,
                                       &mem_read,&mem_write,&cmem3,&cmemw3,&task_ready,c3);

    Processor* CPU4=new Processor(&cdatas4,&caddr4,&cpu_write4,&cpu_read4,&w);
    Cache* c4=new Cache(&clk,&cdatas4,&caddr4,&cpu_read4,&cpu_write4,&cmem4,&cmemw4,&w);
    ControlCache* cc4=new ControlCache(&clk,&mem_addr,&mem_data,&cdatas4,&caddr4,&tt4,
                                       &mem_read,&mem_write,&cmem4,&cmemw4,&task_ready,c4);

    Cola* ControlQueue=new Cola(&busy,&tt2,&tt3,&tt4,&mem_read,&mem_write,&task_ready);
    Memory* memory=new Memory(&clk,&mem_addr,&mem_data,&mem_read,&mem_write,&w);

    //This number is used to print at the gui
    CPU1->setNumber(2);
    c1->setNumber(1);
    CPU2->setNumber(4);
    c2->setNumber(3);
    CPU3->setNumber(6);
    c3->setNumber(5);
    CPU4->setNumber(8);
    c4->setNumber(7);

    //Thread declarations and inizialize
    std::thread tclk_signal(clk_signal);
    std::thread cpu1=CPU1->trun();
    std::thread cpu2=CPU2->trun();
    std::thread cpu3=CPU3->trun();
    std::thread cpu4=CPU4->trun();

    std::thread C1=c1->trun();
    std::thread C2=c2->trun();
    std::thread C3=c3->trun();
    std::thread C4=c4->trun();

    std::thread tcc1=cc1->trun();
    std::thread tcc2=cc2->trun();
    std::thread tcc3=cc3->trun();
    std::thread tcc4=cc4->trun();

    std::thread tcola=ControlQueue->trun();
    std::thread tmem=memory->trun();


    w.show();

    return a.exec();
}
