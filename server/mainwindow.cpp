#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    server * my_server = new server("",8888);

    connect(my_server,SIGNAL(onlineNum(int)),this,SLOT(displayOnline(int)));

    //connect(this,SIGNAL(closeMainWindow()),my_server,SLOT(deleteLater()));


}

MainWindow::~MainWindow()
{
   // emit closeMainWindow();
    delete ui;
}

void MainWindow::displayOnline(int num)
{
    ui->label_online->setText(QString::number(num));
}
