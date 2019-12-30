#ifndef SOCKET_H
#define SOCKET_H
#include <QTcpSocket>
//#include "database.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class socket:public QTcpSocket
{
    Q_OBJECT
public:
    socket(int socketdesc,QTcpSocket *parent = NULL);
    ~socket();
private slots:
    void recieveData();
    void logout();
private:
    QString userID;//连接这个socket的用户名
    QString socketName;//socket标志号
    QSqlDatabase db;//数据库

    void signin(QString,QString);//处理登陆请求
    void signup(QString,QString);//处理注册请求
    void sendAllUserName();//发送所有用户id
    void sendOnlineUserName();//发送在线用户id
    void sendInfo(QString);//发送某个用户信息
    void newHero(QStringList);//生成新的精灵，写到数据库里
    void searchHeroInfo(QStringList,int);//查询某个精灵信息并送给客户端


    void databaseConnection();
    void updateUserInfo(QString);

};

#endif // SOCKET_H
