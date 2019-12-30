#ifndef HERO_BASE_H
#define HERO_BASE_H

#include <QString>
#include <QTime>
#include <QtGlobal>



class hero_base
{
public:
    hero_base();

    int level_up();
    double normal_attack(bool*);
    virtual double special_attack();
    bool is_alive();
    void receive_experience(int);
    double beaten(double,bool*);

    QString get_name();
    int get_id();
    int get_kind();
    int get_level();
    int get_exp();


    double get_atk();
    double get_def();
    double get_hp();
    double get_speed();
    double get_atk_interval();
    double get_critical();
    double get_dodge();

    double get_atk_grow();
    double get_def_grow();
    double get_hp_grow();
    double get_currenthp();
    double get_speed_grow();
    double get_critical_rate_grow();
    double get_dodge_rate_grow();

    void recoverhp();

    void setAttribute(int level,int experience,double attack,double deffence,double hp,double speed,double atk_interval,double critical_rate,double
   dodge_rate,double atk_grow,double def_grow,double hp_grow,double speed_grow,double critical_rate_grow,double dodge_rate_grow);
    void setid(int);
protected:
    /*basic information*/
    QString name;
    int id;
    int kind;//strenth tank defence quick
    int level;
    int experience;
    double attack;
    double deffence;
    double hp;
    double current_hp;
    double speed;
    double atk_interval;
    double critical_rate;
    double dodge_rate;

    /*attribute grow rate, used when level up, stronger and sharper*/
    double atk_grow;
    double def_grow;
    double hp_grow;
    double speed_grow;
    double critical_rate_grow;
    double dodge_rate_grow;

};


class hero_strength:public hero_base
{
public:
    hero_strength();

    double special_attack();
private:

};


class hero_tank:public hero_base
{
public:
    hero_tank();

    double special_attack();
private:

};


class hero_defence:public hero_base
{
public:
    hero_defence();

    double special_attack();
private:

};

class hero_quick:public hero_base
{
public:
    hero_quick();

    double special_attack();
private:

};
#endif // HERO_BASE_H
