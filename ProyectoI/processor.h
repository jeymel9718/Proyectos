#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "mainwindow.h"
#include <thread>
#include <mutex>
#include <condition_variable>

class Processor {
public:
  int State;
  int *addr;
  int *data;

  Processor(int*,int*,bool*,bool*,MainWindow*);

  void run();
  void setNumber(int);
  std::thread trun();
private:
  bool r = 1,imprimir=0,ips=0;
  bool *c_read,*c_write;
  int number;
  MainWindow *mw;
};

#endif //PROCESSOR_H
