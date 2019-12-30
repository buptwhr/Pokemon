#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include "socket.h"
#include <QThread>
class server:public QTcpServer
{
    Q_OBJECT
public:
    server(const std::string &ip, int port, QTcpServer *parent = NULL);
    ~server();

protected:
    void incomingConnection(qintptr socketDescriptor);//有socket连接时自动触发的函数，是对基类的重写

private slots:
    void SocketDisconn();//断开连接时的槽函数
signals:
    void onlineNum(int);
private:
    int user_online;//在线用户数量，为了方便调试，图形化展示出来
};

#endif // SERVER_H
