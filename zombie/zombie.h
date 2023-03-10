#ifndef ZOMBIE_ZOMBIE_H
#define ZOMBIE_ZOMBIE_H

#include "../alien/alien.h"
#include <string>

class Zombie
{
public:
    int x, y;
    int sequence;
    char character;
    int life, attack, range, poison;
    int initialLife, initialAttack, initialRange;
    bool active = false;
    bool isDead = false;
    bool encounterWitch = false;

    Zombie(int no, int X, int Y);
    void Reset();
    void ResetAttack();
    void IncreaseAttack(int atk);
    void TakeDamage(int dmg);
    bool CanAttack(Alien &a);
    void Attack(Alien &a);
    char CheckForCollision(int x, int y);
    std::string GetDirection(int &targetX, int &targetY);
    void Move(int targetX, int targetY);
};

#endif