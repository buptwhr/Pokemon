#include "hero_base.h"

/*strength hero*/
hero_strength::hero_strength()
{
    name = "strength";
    kind = 1;
    attack += 2;
    atk_grow += 1;
}

double hero_strength::special_attack()
{
    return attack * 2.5;
}

/*tank hero*/
hero_tank::hero_tank()
{
    name = "tank";
    kind = 2;
    hp += 10;
    hp_grow += 3;
}

double hero_tank::special_attack()
{
    return hp * 0.3;
}

/*defence hero*/
hero_defence::hero_defence()
{
    name = "deffence";
    kind = 3;
    deffence += 2;
    def_grow += 1;
}

double hero_defence::special_attack()
{
    return attack + deffence;
}

/*quick hero*/
hero_quick::hero_quick()
{
    name = "quick";
    kind = 4;
    speed += 2;
    speed_grow += 1;
}

double hero_quick::special_attack()
{
    return attack + speed;
}
