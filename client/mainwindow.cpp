#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket();

    QString IP;
    int port;

    IP="127.0.0.1";
    port=8888;
    socket->connectToHost(IP,port);

    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadData);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);

    if(!socket->waitForConnected(30000)){
        qDebug()<<"connection failed!";
        ui->pushButton_login->setEnabled(false);
        ui->pushButton_signup->setEnabled(false);
        ui->pushButton_reconnect->setVisible(true);
    }else{
        qDebug()<<"connection success!";
        ui->pushButton_login->setEnabled(true);
        ui->pushButton_signup->setEnabled(true);
        ui->pushButton_reconnect->setVisible(false);
    }

    ui->stackedWidget->setCurrentIndex(0);//show login window

    //my_hero = NULL;
    system_hero = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}


//重新连接服务器
void MainWindow::on_pushButton_reconnect_clicked()
{
    QString IP;
    int port;
    IP="127.0.0.1";
    port=8888;
    socket->connectToHost(IP,port);
    if(!socket->waitForConnected(30000)){
        qDebug()<<"connection failed!";
        ui->pushButton_login->setEnabled(false);
        ui->pushButton_signup->setEnabled(false);
        ui->pushButton_reconnect->setVisible(true);
    }else{
        qDebug()<<"connection success!";
        ui->pushButton_login->setEnabled(true);
        ui->pushButton_signup->setEnabled(true);
        ui->pushButton_reconnect->setVisible(false);
    }
}
/*
 * Follows are using app actively主动向服务器发送请求
 *
 *
 * * 登陆： 1 id password
 * 注册： 2 id passwprd
 * 查看所有用户id： 3
 * 查看在线用户id： 4
 * 查看某个用户信息： 5 id
 * 为某用户生成若干精灵： 6 masterID num_of_new_hero
 *                    name kind level experience attack deffence hp speed atk_intercal critical_rate dodge_grow def_grow hp_grow critical_rate_grow dodge_rate_grow
 *                    ...
 * 查询某个精灵的信息: 7 id
 * 退出登陆 13
 *
 *
 *
 *
 */

//1代表登陆
void MainWindow::on_pushButton_login_clicked()
{
    QString uid = ui->lineEdit->text();
    QString psw = ui->lineEdit_2->text();
    if(uid.length()==0 || psw.length()==0)
    {
        QMessageBox::critical(NULL, "critical", "账号和密码不能为空", QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        QString send = "1 "+uid+" "+psw;
        socket->write(send.toUtf8());
       // ui->stackedWidget->setCurrentIndex(1);
    }
}
//2代表注册
void MainWindow::on_pushButton_signup_clicked()
{
    QString uid = ui->lineEdit->text();
    QString psw = ui->lineEdit_2->text();
    if(uid.length()==0 || psw.length()==0)
    {
        QMessageBox::critical(NULL, "critical", "账号和密码不能为空", QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        QString send = "2 "+uid+" "+psw;
        socket->write(send.toUtf8());
    }
}
//3请求全部用户信息
void MainWindow::on_pushButton_shuaxin1_clicked()
{
    ui->label_3->setText("全部用户");
    QString send = "3 ";
    socket->write(send.toUtf8());
}
//4请求在线用户信息
void MainWindow::on_pushButton_shuaxin2_clicked()
{
    ui->label_3->setText("在线用户");
    QString send = "4 ";
    socket->write(send.toUtf8());
}
//5请求某个用户的详细信息
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString send = "5 ";
    send += item->text();
    socket->write(send.toUtf8());
}

//6给某个用户随机生成n个精灵 写到用户数据库
void MainWindow::randomNewHero(QString uid,int n)
{

    hero_base* my_hero = NULL;


    QString send = "6 ";
    send = send + uid + " " + QString::number(n) + " ";

    for(int i=0;i<n;i++)
    {
        int num = qrand()%4;

        switch (num) {
        case 0:
            my_hero = new hero_strength();
            break;
        case 1:
            my_hero = new hero_quick();
            break;
        case 2:
            my_hero = new hero_defence();
            break;
        default:
            my_hero = new hero_tank();
            break;
        }

        send = send
               + my_hero->get_name() + " "
               + QString::number(my_hero->get_kind()) + " "
               + QString::number(my_hero->get_level()) + " "
               + QString::number(my_hero->get_exp()) + " "
               + QString::number(my_hero->get_atk()) + " "
               + QString::number(my_hero->get_def()) + " "
               + QString::number(my_hero->get_hp()) + " "
               + QString::number(my_hero->get_speed()) + " "
               + QString::number(my_hero->get_atk_interval()) + " "
               + QString::number(my_hero->get_critical()) + " "
               + QString::number(my_hero->get_dodge()) + " "
               + QString::number(my_hero->get_atk_grow()) + " "
               + QString::number(my_hero->get_def_grow()) + " "
               + QString::number(my_hero->get_hp_grow()) + " "
               + QString::number(my_hero->get_speed_grow()) + " "
               + QString::number(my_hero->get_critical_rate_grow()) + " "
               + QString::number(my_hero->get_dodge_rate_grow()) + " ";

        delete my_hero;
        my_hero = NULL;
    }
    socket->write(send.toUtf8());

}
//7 查询某一个小精灵的具体信息
void MainWindow::on_listWidget_3_itemClicked(QListWidgetItem *item)
{
    QString s = item->text();
    QStringList l = s.split(" ");
    QString send = "7 ";
    send += QString::number(l[0].toInt());
    socket->write(send.toUtf8());
}


//13 退出登陆
void MainWindow::on_button_logout_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    QString send = "13 ";
    socket->write(send.toUtf8());
}

/*
 * Follows are using app passively  从服务器接收TCP并处理
 *
 * 回应登陆： 1 0/1/2/3 id num_hero num_superhero num_win num_pk   0密码错误 1登陆成功 2用户不存在 3用户已经存在
 * 回应注册： 2 0/1/2 id    0注册失败 1注册成功 2用户名存在  共7项
 * 送所有用户id: 3 num id id id ...
 * 送在线用户id: 3 num id id id ...
 * 查看某个用户的具体信息：5 id num_hero num_superhero num_win num_pk id1 name1 id2 name2...
 *
 * 送某个精灵的具体信息：7
 *
 *
 *
 *
 *
 */
void MainWindow::socketReadData()
{
    QByteArray buffer;
    buffer = socket->readAll();
    qDebug()<<"收到的全部内容为"<<buffer;
    QString receive_data = buffer;
    QStringList string_list = receive_data.split(" ");

    int function = string_list[0].toInt();
    qDebug()<<"功能代码"<<function;

    switch(function)
    {
    case 1://返回登陆事件
        login(string_list[1].toInt());
        if(string_list[1] == "1")//如果登陆成功，记录用户信息并展示
        {
            my_info = new user_info(string_list[2],true,string_list[3].toInt(),string_list[4].toInt(),string_list[5].toInt(),string_list[6].toInt());
            receiveMyInfo();
        }
        break;
    case 2://返回注册事件
        logup(string_list[1].toInt());
        if(string_list[1] == "1")//如果注册成功，送3只精灵
        {
            randomNewHero(string_list[2],3);
        }
        break;
    case 3://送所有用户
        displayAllUser(string_list);
        break;
    case 4://送在线用户
        break;
    case 5://收到一个用户的信息
        receiveUserInfo(string_list);
        break;
    case 7://收到某个小精灵信息
        receiveHeroInfo(string_list);
        break;
    default:

        break;
    }
}
//1 展示登陆提示
void MainWindow::login(int s)
{
    switch(s)
    {
    case 1:
        QMessageBox::information(NULL, "Title", "登陆成功", QMessageBox::Yes, QMessageBox::Yes);
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case 2:
        QMessageBox::critical(NULL, "critical", "用户名不存在", QMessageBox::Yes, QMessageBox::Yes);
        break;
    case 3:
        QMessageBox::critical(NULL, "critical", "用户已在线", QMessageBox::Yes, QMessageBox::Yes);
        break;
    default:
        QMessageBox::critical(NULL, "critical", "密码错误", QMessageBox::Yes, QMessageBox::Yes);
        break;
    }
}
//2 展示注册提示
void MainWindow::logup(int s)
{
    switch(s)
    {
    case 1:
        QMessageBox::information(NULL, "Title", "注册成功", QMessageBox::Yes, QMessageBox::Yes);
        //ui->stackedWidget->setCurrentIndex(1);
        break;
    case 2:
        QMessageBox::critical(NULL, "critical", "用户名已存在", QMessageBox::Yes, QMessageBox::Yes);
        break;
    default:
        QMessageBox::critical(NULL, "critical", "注册失败", QMessageBox::Yes, QMessageBox::Yes);
        break;
    }
}
//登陆成功后 更新用户对象保存用户信息
void MainWindow::receiveMyInfo()
{
    ui->user_d_1->setText(my_info->get_id());
    ui->user_d_2->setText(QString::number(my_info->get_num_hero()));
    ui->user_d_3->setText(QString::number(my_info->get_num_superhero()));
    ui->user_d_4->setText(QString::number(my_info->get_num_win()));
    ui->user_d_5->setText(QString::number(my_info->get_num_pk()));
    ui->user_d_6->setText(QString::number(my_info->get_winrate()));
    switch (my_info->get_medal()) {
    case 1:
        ui->user_d_7->setText("金");
        break;
    case 2:
        ui->user_d_7->setText("银");
        break;
    case 3:
        ui->user_d_7->setText("铜");
        break;
    default:
        ui->user_d_7->setText("无");
        break;
    }

    switch (my_info->get_supermedal()) {
    case 1:
        ui->user_d_8->setText("金");
        break;
    case 2:
        ui->user_d_8->setText("银");
        break;
    case 3:
        ui->user_d_8->setText("铜");
        break;
    default:
        ui->user_d_8->setText("无");
        break;
    }
}
//3 or 4 显示在线用户或全部用户
void MainWindow::displayAllUser(QStringList s)
{
    ui->listWidget->clear();
    int num = s[1].toInt();
    for(int i =0; i<num; i++)
    {
        ui->listWidget->addItem(s[2+i]);
    }
}
//5 接收展示用户信息
void MainWindow::receiveUserInfo(QStringList l)
{
    user_info* some_info = new user_info(l[1],0,l[2].toInt(),l[3].toInt(),l[4].toInt(),l[5].toInt());
    ui->some_d_1->setText(some_info->get_id());
    ui->some_d_2->setText(QString::number(some_info->get_num_hero()));
    ui->some_d_3->setText(QString::number(some_info->get_num_superhero()));
    ui->some_d_4->setText(QString::number(some_info->get_num_win()));
    ui->some_d_5->setText(QString::number(some_info->get_num_pk()));
    ui->some_d_6->setText(QString::number(some_info->get_winrate()));
    switch (some_info->get_medal()) {
    case 1:
        ui->some_d_7->setText("金");
        break;
    case 2:
        ui->some_d_7->setText("银");
        break;
    case 3:
        ui->some_d_7->setText("铜");
        break;
    default:
        ui->some_d_7->setText("无");
        break;
    }

    switch (some_info->get_supermedal()) {
    case 1:
        ui->some_d_8->setText("金");
        break;
    case 2:
        ui->some_d_8->setText("银");
        break;
    case 3:
        ui->some_d_8->setText("铜");
        break;
    default:
        ui->some_d_8->setText("无");
        break;
    }
    ui->listWidget_3->clear();

    for(int i=0 ;i<l[2].toInt();i++)
    {
        QString list;
        list = l[6+i*2] + " " + l[7+i*2];
        ui->listWidget_3->addItem(list);

    }

    if(some_info->get_id() == my_info->get_id())
    {
        ui->listWidget_5->clear();
        for(int i=0 ;i<l[2].toInt();i++)
        {
            QString list;
            list = l[6+i*2] + " " + l[7+i*2];
            ui->listWidget_5->addItem(list);
        }
    }

    if(some_info->get_id() == "system")
    {
        ui->listWidget_6->clear();
        for(int i=0 ;i<l[2].toInt();i++)
        {
            QString list;
            list = l[6+i*2] + " " + l[7+i*2];
            ui->listWidget_6->addItem(list);
        }
    }
    delete some_info;
}
//7 接收展示精灵信息
void MainWindow::receiveHeroInfo(QStringList l)
{
    ui->lh0->setText(l[1]);
    ui->lh1->setText(l[3]);
    ui->lh2->setText(l[5]);
    ui->lh3->setText(l[7]);
    ui->lh4->setText(l[8]);
    ui->lh5->setText(l[9]);
    ui->lh6->setText(l[12]);
    ui->lh7->setText(l[13]);
}








/* 处理QT界面按钮
 * 简单的图形化功能
 *
 *
 */
//进入社交界面
void MainWindow::on_button_friend_clicked()
{
    ui->listWidget->clear();
    ui->listWidget_3->clear();
    ui->stackedWidget->setCurrentIndex(2);
}
//社交界面返回主页
void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


//断开连接时，登陆界面显示重连信息，使登陆注册按钮失效
void MainWindow::socketDisconnected()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->pushButton_login->setEnabled(false);
    ui->pushButton_signup->setEnabled(false);
    ui->pushButton_reconnect->setVisible(true);
    QMessageBox::critical(NULL, "critical", "与服务器断开连接", QMessageBox::Yes, QMessageBox::Yes);

}

