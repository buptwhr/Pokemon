#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include "user_info.h"
#include "hero_base.h"
#include <QListWidgetItem>
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
    void on_pushButton_login_clicked();//点击登陆按钮

    void on_pushButton_signup_clicked();//点击注册按钮

    void socketReadData();//读取socket数据

    void socketDisconnected();//断开连接操作

    void on_button_friend_clicked();//点击社交功能按钮

    void on_pushButton_reconnect_clicked();//登陆界面重连服务器按钮

    void on_pushButton_shuaxin1_clicked();//刷新全部用户

    void on_pushButton_shuaxin2_clicked();//刷新在线用户

    void on_listWidget_itemClicked(QListWidgetItem *item);//请求某个用户的详细信息

    void on_listWidget_3_itemClicked(QListWidgetItem *item);//查询某个小精灵的详细信息

    void on_pushButton_clicked();//返回主页

    void on_button_logout_clicked();//退出登陆

private:
    Ui::MainWindow *ui;//ui界面
    QTcpSocket *socket;//socket
    user_info* my_info;//用户信息

   // hero_base* my_hero;
    hero_base* system_hero;

    void login(int);//登陆
    void logup(int);//注册
    void displayAllUser(QStringList);//显示所有用户名字
    void receiveMyInfo();//接收用户的个人信息，更新用户对象
    void receiveUserInfo(QStringList);//更新某个用户信息
    void receiveHeroInfo(QStringList);//接收某个小精灵的信息
    void randomNewHero(QString,int);//随机生成n个小精灵


};

#endif // MAINWINDOW_H
