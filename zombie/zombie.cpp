#include "../dependencies.h"

Zombie::Zombie(int no, int X, int Y)
{
    x = X;
    y = Y;
    sequence = no;
    character = 48 + no;
    poison = 0;

    life = 100 + (rand() % 101);
    attack = 5 + (rand() % 20);
    range = 1 + (rand() % 10);

    initialLife = life;
    initialAttack = attack;
    initialRange = range;
}

void Zombie::Reset()
{
    isDead = false;
    active = false;
    encounterWitch = false;
    poison = 0;
    life = initialLife;
    attack = initialAttack;
    range = initialRange;
}

void Zombie::ResetAttack()
{
    attack = initialAttack;
}

void Zombie::IncreaseAttack(int atk)
{
    attack += atk;
}

void Zombie::TakeDamage(int dmg)
{
    life -= dmg;

    if (life <= 0)
    {
        isDead = true;
        life = 0;
        pf::b[y][x] = g_empty;
    }
}

bool Zombie::CanAttack(Alien &a)
{
    // calculate distance between zombie and alien
    int diffX = abs(x - a.x);
    int diffY = abs(y - a.y);
    int distance = diffY + diffX;

    return (distance <= range);
}

void Zombie::Attack(Alien &a)
{
    a.TakeDamage(attack);
}

string Zombie::GetDirection(int &targetX, int &targetY)
{
    string direction;
    char col;
    int random_number;
    do
    {
        random_number = rand() % 4;

        switch (random_number)
        {
        case 0:
            direction = g_up;
            targetX = x;
            targetY = y - 1;
            break;
        case 1:
            direction = g_down;
            targetX = x;
            targetY = y + 1;
            break;
        case 2: 
            direction = g_left;
            targetX = x - 1;
            targetY = y;
            break;
        case 3: 
            direction = g_right;
            targetX = x + 1;
            targetY = y;
            break;
        }

        col = CheckForCollision(targetX, targetY);
    } while (col);

    return direction;
}

char Zombie::CheckForCollision(int targetX, int targetY)
{
    char col = '\0';

    // check if it is out of border
    if (targetX < 0 || targetX > pf::b[0].size() - 1 || targetY < 0 || targetY > pf::b.size() - 1)
    {
        return 'b';
    }

    char obj = pf::b[targetY][targetX];

    // check if it collides with other zombie
    for (int i = 0; i < pf::zombiesArray.size(); ++i)
    {
        if (obj == pf::zombiesArray[i].character)
        {
            return pf::zombiesArray[i].character;
        }
    }

    if (obj == g_alien)
    {
        return g_alien;
    }

    if (obj == g_witch)
    {
        encounterWitch = true;
    }

    return col;
}

void Zombie::Move(int targetX, int targetY)
{
    pf::b[y][x] = g_empty;
    x = targetX;
    y = targetY;
    pf::b[y][x] = character;
}