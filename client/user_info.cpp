#include "user_info.h"

user_info::user_info()
{

}

user_info::user_info(QString id,bool state, int num_hero, int num_superhero, int num_win, int num_pk)
{
    this->id = id;
    this->state = state;
    this->num_hero = num_hero;
    this->num_superhero = num_superhero;
    this->num_win = num_win;
    this->num_pk = num_pk;

    this->my_hero = NULL;
    update();

}

void user_info::update()
{
    winrate = (double)num_win/num_pk;

    if(num_hero<3)
        this->medal = 0;
    else if(num_hero < 6)
        this->medal = 3;
    else if(num_hero < 10)
        this->medal = 2;
    else
        this->medal = 1;

    if(num_superhero<3)
        this->supermedal = 0;
    else if(num_superhero < 6)
        this->supermedal = 3;
    else if(num_superhero < 10)
        this->supermedal = 2;
    else
        this->supermedal = 1;
}

void user_info::changeNum(bool y)
{
    if(y)
        num_hero++;
    else
        num_hero--;
    update();
}
void user_info::changeSuper(bool y)
{
    if(y)
    {
        num_hero++;
        num_superhero++;
    }
    else{
        num_hero--;
        num_superhero--;
    }
    update();

}

