#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <mutex>
#include <iostream>

extern std::mutex mtx;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    counter=0;
    //inicializa la vista
    for(int i=0;i<16;++i){
        ui->listWidget->addItem(QString::number(i)+" | S | 0");
        ui->cache2->addItem(QString::number(i)+" | S | 0");
        ui->cache3->addItem(QString::number(i)+" | S | 0");
        ui->cache4->addItem(QString::number(i)+" | S | 0");
        ui->memory->addItem(QString::number(i)+" | 0");
    }

}

void MainWindow::UpdateView(int view,int pos,QString value){
    mtx.lock();
    switch (view) {
        case 1:
            ui->listWidget->item(pos)->setText(value);
            break;
       case 2:
            ui->label->setText(value);
            break;
       case 3:
            ui->cache2->item(pos)->setText(value);
            break;
       case 4:
            ui->cpu2->setText(value);
            break;
       case 5:
            ui->cache3->item(pos)->setText(value);
            break;
       case 6:
            ui->cpu3->setText(value);
            break;
       case 7:
            ui->cache4->item(pos)->setText(value);
            break;
       case 8:
            ui->cpu4->setText(value);
            break;
       case 9:
            ui->memory->item(pos)->setText(value);
            break;

    }
    mtx.unlock();
}


void MainWindow::addClk(){
    counter=++counter;
    ui->label_3->setText(QString::number(counter));
}

void MainWindow::nextClk(){
    std::cout<<"entro"<<'\n';
}

MainWindow::~MainWindow()
{
    delete ui;
}
