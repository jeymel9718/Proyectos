#ifndef BUS_H
#define BUS_H

#include <thread>
#include <QVector>

class Cola
{
public:
    Cola(bool*,bool*,bool*,bool*,bool*,bool*,bool*);
    void run();
    std::thread trun();
    void process_signal();

private:
    QVector<bool*> controls; //señales de los controladores de cache
    bool *ready,*mem_read,*mem_write; //señales de control de la memoria
    QVector<int> queue; //cola de transacciones
};

#endif // BUS_H
