#ifndef USER_INFO_H
#define USER_INFO_H
#include <QMainWindow>
#include <hero_base.h>
class user_info
{
public:
    user_info();
    user_info(QString,bool,int,int,int,int);
    //更新胜率
    void update();
    QString get_id(){return id;};
    bool get_state(){return state;};
    int get_num_hero(){return num_hero;};
    int get_num_superhero(){return num_superhero;};
    int get_num_win(){return num_win;};
    int get_num_pk(){return num_pk;};
    double get_winrate(){return winrate;};
    int get_medal(){return medal;};
    int get_supermedal(){return supermedal;};
    hero_base* my_hero;//参与战斗的我的小精灵
    hero_base* get_my_hero(){return my_hero;};
    void set_my_hero(hero_base* h){my_hero = h;};
    //更新胜场
    void playone(){num_pk++;update();};
    void winone(){num_win++;update();};
    //改变精灵数量
    void changeNum(bool);
    void changeSuper(bool);


private:

    QString id;//用户名
    bool state;//状态
    int num_hero;//精灵数量
    int num_superhero;//满级精灵数量
    int num_win;//战斗胜利次数
    int num_pk;//战斗次数
    double winrate;//胜率
    int medal;//0没有勋章 1金 2银 3铜
    int supermedal;//0没有 1金 2银 3铜




};

#endif // USER_INFO_H
