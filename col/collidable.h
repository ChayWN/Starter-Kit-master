#ifndef COL_COLLIDABLE_H
#define COL_COLLIDABLE_H

#include "../zombie/zombie.h"

namespace col
{
    extern char underRock;
    extern int zombieToPodIndex;

    void AlienWitchEncounter();
    void ZombieWitchEncounter(Zombie& z);
    void OnCollide(char col, int x, int y);
}

#endif