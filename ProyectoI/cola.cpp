#include "cola.h"

Cola::Cola(bool* cc1,bool* cc2,bool *cc3,bool* cc4,bool* read_,bool* write_,bool *ready_){
    this->controls.push_back(cc1);
    this->controls.push_back(cc2);
    this->controls.push_back(cc3);
    this->controls.push_back(cc4);
    this->mem_read=read_;
    this->mem_write=write_;
    this->ready=ready_;
}

void Cola::run(){
    while(true){
        for(int i=0;i<4;++i){
            if(*controls[i] && !queue.contains(i)){ //se añade a la cola si no esta
                queue.push_back(i);
            }
        }
        std::this_thread::sleep_for (std::chrono::seconds(1));
        process_signal();
    }
}

std::thread Cola::trun(){
  return std::thread([=]{run();});
}

void Cola::process_signal(){
    if(!*mem_read && !*mem_write){ //la memoria esta libre
        if(!queue.isEmpty() && !*ready){ //hay tareas pendientes
            *controls.at(queue.first())=0;
            queue.pop_front();
            *ready=1;
        }
    }
}

