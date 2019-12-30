#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "database.h"
//#include "socket.h"
#include "server.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void displayOnline(int);
signals:
  //  void closeMainWindow();
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
