#ifndef ALIEN_ALIEN_H
#define ALIEN_ALIEN_H

#include <string>

class Alien 
{
public:
    int x, y;
    char character;
    int maxLife, life, attack, poison, fireball;
    bool active = true;
    bool isDead = false;
    bool triggerPoison = false;

    Alien();
    void Reset();
    void Heal(int health);
    void TakeDamage(int dmg);
    void ResetAttack();
    void IncreaseFireball();
    void DecreaseFireball();
    void IncreaseAttack(int atk);
    bool Collide(char col, int targetX, int targetY);
    char CheckForCollision(int targetX, int targetY);
    void GetNextCoord(std::string direction, int& rTargetX, int& rTargetY);
    void Move(int targetX, int targetY);
};


#endif