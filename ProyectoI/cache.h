#ifndef CACHE_H
#define CACHE_H

#include <thread>
#include <mutex>
#include "mainwindow.h"

class Cache{
public:
  int mem[16]={}; //memoria cache
  char mem_s[16]={'s','s','s','s','s','s','s','s','s','s','s','s','s','s','s','s'}; //estado de cada bloque de la memoria cache
  void run();
  std::thread trun();

  Cache(int*,int*,int*,bool*,bool*,bool*,bool*,MainWindow*);
  void setNumber(int);
  void updateCachei(int);
  void updateCaches(int);
private:
  //señales de control del cpu
  bool* cpu_read,*cpu_write;
  //señales de control del controlcache
  bool* mem_read,*mem_write;
  //bus de datos del cache
  int* data,*addr;
  //indica si se debe esperar a la memoria
  bool waiting=0;
  MainWindow* mw;
  int numero; //numero de la GUI
  int* clk;
  bool clk_aux;
};

#endif //CACHE_H
