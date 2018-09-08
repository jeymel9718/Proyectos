#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void UpdateView(int,int,QString);
    void addClk();

    ~MainWindow();

private slots:
    void nextClk();

private:
    int counter=0;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
