#include "socket.h"
#include <qthread.h>
#include <qlist.h>
#include <time.h>
/*
 * ---------------The formation of TCP---------------
 *
 * ------------------client->server------------------
 * 登陆： 1 id password
 * 注册： 2 id passwprd
 * 查看所有用户id： 3
 * 查看在线用户id： 4
 * 查看某个用户信息： 5 id
 * 为某用户生成若干精灵： 6 masterID num_of_new_hero
 *                    name kind level experience attack deffence hp speed atk_intercal critical_rate dodge_grow def_grow hp_grow critical_rate_grow dodge_rate_grow
 *                    ...
 * 查询某个精灵的信息: 7 id
 *
 *
 * ------------------server->client------------------
 * 回应登陆： 1 0/1/2/3 id num_hero num_superhero num_win num_pk   0密码错误 1登陆成功 2用户不存在 3用户已经存在
 * 回应注册： 2 0/1/2 id    0注册失败 1注册成功 2用户名存在  共7项
 * 送所有用户id: 3 num id id id ...
 * 送在线用户id: 3 num id id id ...
 * 查看某个用户的具体信息：5 id num_hero num_superhero num_win num_pk id1 name1 id2 name2...
 *
 * 送某个精灵的具体信息：7 19项数据库信息
 *
 *
 */

socket::socket(int sock,QTcpSocket *parent):QTcpSocket(parent)
{
    this->setSocketDescriptor(sock);
    socketName = "db";
    int currentTime = time(NULL);
    socketName += QString::number(currentTime);
    userID = "";
}

socket::~socket()
{
}

//read data from socket
void socket::recieveData()
{
    QByteArray buffer;
    buffer = this->readAll();
    qDebug()<<"收到的全部内容为"<<buffer;
    QString receive_data = buffer;
    QStringList string_list = receive_data.split(" ");

    int function = string_list[0].toInt();
    qDebug()<<"功能代码"<<function;

    switch(function)
    {
    case 1:
        signin(string_list[1],string_list[2]);
        break;
    case 2:
        signup(string_list[1],string_list[2]);
        break;
    case 3:
        sendAllUserName();
        break;
    case 4:
        sendOnlineUserName();
        break;
    case 5:
        sendInfo(string_list[1]);
        break;
    case 6://数据库插入小精灵
        newHero(string_list);
        break;
    case 7://数据库查某个小精灵的信息
        searchHeroInfo(string_list,7);
        break;

    case 13:
        logout();
    default:

        break;
    }

}
//1
void socket::signin(QString uid, QString psw)
{
    QString send = "1 ";
    databaseConnection();
    if(!db.open())
    {
        qDebug()<<"Open database failed!";
    }
    else
    {
        qDebug()<<"open database successfully!";
    }

    QSqlQuery query(QSqlDatabase::database(socketName));
    QString s = "select password from user where id= ?";
    query.prepare(s);
    query.addBindValue(uid);
    query.exec();
    query.last();
    int record = query.at();

    if(record != 0)
    {
        send += "2";
        qDebug()<<"用户不存在";
    }
    else
    {
        QString psw_real = query.value(0).toString();
        if(psw == psw_real)
        {

            s = "select state from user where id = :id";
            query.prepare(s);
            query.bindValue(":id",uid);
            query.exec();

            query.next();
            int state = query.value(0).toInt();

            if(state == 0)
            {
                userID = uid;
                send += "1";
                qDebug()<<"登陆成功";

                s = "update user set state=1 where id= :id";
                query.prepare(s);
                query.bindValue(":id",uid);
                query.exec();


                s = "select num_hero, num_superhero, num_win, num_pk from user where id = :id";
                query.prepare(s);
                query.bindValue(":id",userID);
                query.exec();
                query.next();

                send = send + " " + userID + " "
                            + QString::number(query.value(0).toInt()) + " "
                            + QString::number(query.value(1).toInt()) + " "
                            + QString::number(query.value(2).toInt()) + " "
                            + QString::number(query.value(3).toInt());

                qDebug()<<send;
            }
            else
            {
               send += "3";
               qDebug()<<"该用户已经在线";
            }
        }
        else
        {
            send += "0";
            qDebug()<<"密码错误";
        }
    }

    this->write(send.toUtf8());

}
//2
void socket::signup(QString uid, QString psw)
{
    databaseConnection();
    QString send = "2 ";
    if(!db.open())
    {
        qDebug()<<"Open database failed!";
    }
    else
    {
        qDebug()<<"open database successfully!";
    }

    QSqlQuery query(QSqlDatabase::database(socketName));
    QString s = "select * from user where id= ?";
    query.prepare(s);
    query.addBindValue(uid);
    query.exec();
    query.last();
    int record = query.at();

    if(record==0)
    {
        send += "2";
        qDebug()<<"用户名存在";
    }
    else
    {
        s ="insert into user values (?, ?, 0, 0, 0, 0, 0)";
        query.prepare(s);
        query.addBindValue(uid);
        query.addBindValue(psw);
        if(!query.exec())
        {
            send += "0";
            qDebug()<<"注册失败";
        }
        else
        {
            userID = uid;
            send += "1 ";
            send += uid;
            qDebug()<<"注册成功";
        }
    }
    this->write(send.toUtf8());
}

void socket::logout()
{
    QSqlQuery query(QSqlDatabase::database(socketName));
    QString s = "update user set state=0 where id= :id";
    query.prepare(s);
    query.bindValue(":id",userID);
    query.exec();
}
//3
void socket::sendAllUserName()
{
    QSqlQuery query(QSqlDatabase::database(socketName));
    QString s = "select id from user";
    query.prepare(s);
    query.exec();

    QString send = "";
    int num = 0;
    while(query.next())
    {
        num ++;
        send = send + " " + query.value(0).toString();
    }
    send = "3 " + QString::number(num) + send;
    qDebug()<<send;
    this->write(send.toUtf8());
}
//3
void socket::sendOnlineUserName()
{
    QSqlQuery query(QSqlDatabase::database(socketName));
    QString s = "select id from user where state=1";
    query.prepare(s);
    query.exec();

    QString send = "";
    int num = 0;
    while(query.next())
    {
        num ++;
        send = send + " " + query.value(0).toString();
    }
    send = "3 " + QString::number(num) + send;
    qDebug()<<send;
    this->write(send.toUtf8());
}

void socket::databaseConnection()
{
    if (QSqlDatabase::contains(socketName))
    {
        qDebug()<<"数据库连接存在，打开";
        db = QSqlDatabase::database(socketName);
    }
    else
    {   qDebug()<<"数据库连接不存在，建立";
        db = QSqlDatabase::addDatabase("QSQLITE",socketName);
        qDebug()<<socketName;
        db.setHostName("127.0.0.1");
        db.setDatabaseName("/Users/whr/Documents/database/testDB.db");
    }
}
//5
void socket::sendInfo(QString oneid)
{
    QSqlQuery query(QSqlDatabase::database(socketName));
    QString send = "5 ";
    QString s = "select num_hero, num_superhero, num_win, num_pk from user where id = :id";
    query.prepare(s);
    query.bindValue(":id",oneid);
    query.exec();
    query.next();

    send = send + oneid + " "
                + QString::number(query.value(0).toInt()) + " "
                + QString::number(query.value(1).toInt()) + " "
                + QString::number(query.value(2).toInt()) + " "
                + QString::number(query.value(3).toInt()) + " ";



    s = "select id,name from heros where masterID = :id";
    query.prepare(s);
    query.bindValue(":id",oneid);
    query.exec();
    while(query.next())
    {
        send += query.value(0).toString() + " " + query.value(1).toString() + " ";
    }

    this->write(send.toUtf8());
    qDebug()<<send;
}

void socket::newHero(QStringList l)
{
    int n = l[2].toInt();
    QSqlQuery query(QSqlDatabase::database(socketName));
    QString s;

    for(int i=0;i<n;i++)
    {
        s = "insert into heros values (NULL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        query.prepare(s);
        query.addBindValue(l[1]);
        query.addBindValue(l[17*i+3]);
        query.addBindValue(l[17*i+4]);
        query.addBindValue(l[17*i+5]);
        query.addBindValue(l[17*i+6]);
        query.addBindValue(l[17*i+7]);
        query.addBindValue(l[17*i+8]);
        query.addBindValue(l[17*i+9]);
        query.addBindValue(l[17*i+10]);
        query.addBindValue(l[17*i+11]);
        query.addBindValue(l[17*i+12]);
        query.addBindValue(l[17*i+13]);
        query.addBindValue(l[17*i+14]);
        query.addBindValue(l[17*i+15]);
        query.addBindValue(l[17*i+16]);
        query.addBindValue(l[17*i+17]);
        query.addBindValue(l[17*i+18]);
        query.addBindValue(l[17*i+19]);
        query.exec();

    }
    //1是用户id  2是增加精灵数量
    s = "select num_hero from user where id=?";
    query.prepare(s);
    query.addBindValue(l[1]);
    query.exec();
    query.next();
    int oldnum = query.value(0).toInt();
    oldnum += l[2].toInt();

    s = "update user set num_hero=? where id=?";
    query.prepare(s);
    query.addBindValue(QString::number(oldnum));
    query.addBindValue(l[1]);
    query.exec();

}
//7,8,9
void socket::searchHeroInfo(QStringList l,int n)
{
    QString s= "select * from heros where id=?";
    QSqlQuery query(QSqlDatabase::database(socketName));
    query.prepare(s);
    query.addBindValue(l[1]);
    query.exec();
    query.next();

    QString send = QString::number(n) + " ";
    send = send
            + query.value(0).toString() + " "
            + query.value(1).toString() + " "
            + query.value(2).toString() + " "
            + query.value(3).toString() + " "
            + query.value(4).toString() + " "
            + query.value(5).toString() + " "
            + query.value(6).toString() + " "
            + query.value(7).toString() + " "
            + query.value(8).toString() + " "
            + query.value(9).toString() + " "
            + query.value(10).toString() + " "
            + query.value(11).toString() + " "
            + query.value(12).toString() + " "
            + query.value(13).toString() + " "
            + query.value(14).toString() + " "
            + query.value(15).toString() + " "
            + query.value(16).toString() + " "
            + query.value(17).toString() + " "
            + query.value(18).toString() + " ";
    this->write(send.toUtf8());
    qDebug()<<send;
}
