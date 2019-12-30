#include "hero_base.h"

hero_base::hero_base()
{
    name="default";

    id = 0;

    kind=0;
    level=1;
    experience=0;

    attack=10;
    deffence=10;
    hp=100;
    speed=10;
    atk_interval=2;
    critical_rate=0.05;
    dodge_rate=0.05;
    /*attribute grow rate, used when level up, stronger and sharper*/
    atk_grow=5;
    def_grow=5;
    hp_grow=10;
    speed_grow=2;
    critical_rate_grow=0.02;
    dodge_rate_grow=0.01;

    current_hp = hp;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

}

double hero_base::normal_attack(bool* iscritical)
{

    //伤害的1/5和护甲比较  大于0 造成1/5 * 攻击 - 护甲 伤害 否则 0伤害
    double cri = qrand() % 100;
    if( cri/100 < critical_rate )
    {
        *iscritical = true;
        return attack * 2;
    }
    else
    {
        *iscritical = false;
        return attack;
    }
}

double hero_base::special_attack()
{
    //this is a virtual function
}

int hero_base::level_up()
{
    if(level == 15)
        return 0;
    else
    {
        level++;
        attack += atk_grow;
        deffence += def_grow;
        hp += hp_grow;
        speed += speed_grow;
        critical_rate += critical_rate_grow;
        dodge_rate += dodge_rate_grow;
        atk_interval = 20 / speed;
        return level;
    }
}

void hero_base::receive_experience(int exp)
{   //升级需要300*level 的经验
    experience += exp;
    while(experience > 300 * level)
    {
        experience -= 300 * level;
        level_up();
    }
}

double hero_base::beaten(double d,bool* ismiss)
{
    double miss = qrand() % 100;
    double real_damage = 0;


    real_damage = d*4/5;
    if(d/5 > deffence)
        real_damage += d/5 - deffence;


    if( miss/100 < dodge_rate )
    {
        *ismiss = true;
    }
    else
    {
        *ismiss = false;
        current_hp -= real_damage;
    }
    return real_damage;
}

bool hero_base::is_alive()
{
    if(current_hp>0)
        return true;
    else
        return false;
}
QString hero_base::get_name()
{
    return name;
}
int hero_base::get_id()
{
    return id;
}
int hero_base::get_kind()
{
    return kind;
}

int hero_base::get_level()
{
    return level;
}

int hero_base::get_exp()
{
    return experience;
}

double hero_base::get_atk()
{
    return attack;
}

double hero_base::get_def()
{
    return deffence;
}

double hero_base::get_hp()
{
    return hp;
}

double hero_base::get_currenthp()
{
    return current_hp;
}
double hero_base::get_speed()
{
    return speed;
}

double hero_base::get_atk_interval()
{
    return atk_interval;
}

double hero_base::get_critical()
{
    return critical_rate;
}

double hero_base::get_dodge()
{
    return dodge_rate;
}


double hero_base::get_atk_grow()
{
    return atk_grow;
}

double hero_base::get_def_grow()
{
    return def_grow;
}
double hero_base::get_hp_grow()
{
    return hp_grow;
}
double hero_base::get_speed_grow()
{
    return speed_grow;
}
double hero_base::get_critical_rate_grow()
{
    return critical_rate_grow;
}
double hero_base::get_dodge_rate_grow()
{
    return dodge_rate_grow;
}

void hero_base::setAttribute(int level, int experience, double attack, double deffence, double hp, double speed, double atk_interval,
                             double critical_rate, double dodge_rate, double atk_grow, double def_grow,
                             double hp_grow, double speed_grow, double critical_rate_grow, double dodge_rate_grow)
{
    this->level = level;
    this->experience = experience;
    this->attack = attack;
    this->deffence = deffence;
    this->hp = hp;
    this->speed = speed;
    this->atk_interval = atk_interval;
    this->critical_rate = critical_rate;
    this->dodge_rate = dodge_rate;
    this->atk_grow = atk_grow;
    this->def_grow = def_grow;
    this->hp_grow = hp_grow;
    this->speed_grow = speed_grow;
    this->critical_rate_grow = critical_rate_grow;
    this->dodge_rate_grow = dodge_rate_grow;

    this->current_hp = hp;
}

void hero_base::setid(int id)
{
    this->id = id;
}

void hero_base::recoverhp()
{
    current_hp = hp;
}




