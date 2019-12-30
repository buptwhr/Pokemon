#include "server.h"

server::server(const std::string &ip, int port, QTcpServer *parent): QTcpServer(parent)
{
    if(!this->listen(QHostAddress::Any,port)){
        qDebug()<<this->errorString();
    }
    else
    {
        qDebug()<<"listening...";
    }
    user_online = 0;
    //数据库初始化，对所有用户进行下线处理，防止上一次服务器不正常退出
    QSqlDatabase db;
    if (QSqlDatabase::contains("init"))
    {
        qDebug()<<"数据库连接存在，打开";
        db = QSqlDatabase::database("init");
    }
    else
    {   qDebug()<<"数据库连接不存在，建立";
        db = QSqlDatabase::addDatabase("QSQLITE","init");
        db.setHostName("127.0.0.1");
        db.setDatabaseName("/Users/whr/Documents/database/testDB.db");
    }
    if(!db.open())
    {
        qDebug()<<"Open database failed!";
    }
    else
    {
        qDebug()<<"open database successfully!";
    }
    QSqlQuery query(QSqlDatabase::database("init"));

    QString s = "update user set state = 0";

    query.prepare(s);
    query.exec();


    //更新所有用户的小精灵数量信息，防止在后台操纵数据库之后，忘记更改用户拥有精灵数量

    s = "select id from user";
    query.prepare(s);
    query.exec();

    QStringList list;

    while(query.next())
    {
        list.append(query.value(0).toString());
    }

    for (int i=0; i<list.length(); i++)
    {
        QString id = list[i];
        s = "select level from heros where masterID=?";
        query.prepare(s);
        query.addBindValue(id);
        query.exec();
        int num_hero=0;
        int num_superhero=0;

        while(query.next())
        {
            num_hero++;
            int level = query.value(0).toInt();
            if(level == 15)
                num_superhero++;
        }

        s = "update user set num_hero=?,num_superhero=? where id=?";
        query.prepare(s);
        query.addBindValue(QString::number(num_hero));
        query.addBindValue(QString::number(num_superhero));
        query.addBindValue(id);
        query.exec();
    }
}



server::~server()
{

    this->close();


    if (QSqlDatabase::contains("serverclose"))
    {
        qDebug()<<"数据库连接存在，打开";
        QSqlDatabase db = QSqlDatabase::database("serverclose");
    }
    else
    {   qDebug()<<"数据库连接不存在，建立";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","serverclose");

        db.setHostName("127.0.0.1");
        db.setDatabaseName("/Users/whr/Documents/database/testDB.db");
    }

    QSqlQuery query(QSqlDatabase::database("serverclose"));
    QString s = "update user set state = 0";
    query.prepare(s);
    query.exec();
    qDebug()<<"服务器关闭，所有用户强制下线";

}

void server::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"one client connected";
    socket * my_socket = new socket(socketDescriptor);

    connect(my_socket,SIGNAL(readyRead()),my_socket,SLOT(recieveData()));
    connect(my_socket,SIGNAL(disconnected()),this,SLOT(SocketDisconn()));
    QThread * my_thread = new QThread();
    connect(my_socket, SIGNAL(disconnected()), my_thread, SLOT(quit()));
    connect(my_socket, SIGNAL(disconnected()), my_socket,SLOT(logout()));
    connect(my_thread, SIGNAL(finished()), my_thread, SLOT(deleteLater()));

    my_socket->moveToThread(my_thread);
    my_thread->start();

    emit newConnection();

    user_online++;
    emit onlineNum(user_online);
}


void server::SocketDisconn()
{
     user_online--;
     emit onlineNum(user_online);
}


