#include "../dependencies.h"

Alien::Alien()
{
    character = g_alien;
    life = 100 + (rand() % 100);
    maxLife = life;
    attack = 0;
    poison = 0;
    fireball = 0;
}

void Alien::Reset()
{
    isDead = false;
    active = true;
    triggerPoison = false;
    life = maxLife;
    poison = 0;
    fireball = 0;
    ResetAttack();
}

void Alien::Heal(int health)
{
    life += 20;

    if (life > maxLife)
    {
        life = maxLife;
    }
}

void Alien::ResetAttack()
{
    attack = 0;
}

void Alien::IncreaseAttack(int atk)
{
    attack += atk;
}

void Alien::IncreaseFireball()
{
    fireball++;
}

void Alien::DecreaseFireball()
{
    fireball--;
}

void Alien::TakeDamage(int dmg)
{
    life -= dmg;

    if (life <= 0)
    {
        isDead = true;
        life = 0;
    }
}

bool Alien::Collide(char col, int targetX, int targetY)
{
    bool canMove = true;

    if (int(col) > 48 && int(col) < 58) // ASCII: 49-57 = 1-9
    {
        for (int i = 0; i < pf::zombiesArray.size(); ++i)
        {   
            if (col == pf::zombiesArray[i].character)
            {
                Zombie& z = pf::zombiesArray[i];
                z.TakeDamage(attack);

                if (z.isDead) return true;
                else return false;
            }
        }
    }
    
    if (col == 'b' || col == g_rock)
    {
        canMove = false;
    }

    if (col == g_hangman)
    {
        return hangman::PlayHangmanGame(targetX, targetY);
    }

    col::OnCollide(col, targetX, targetY);

    return canMove;
}

char Alien::CheckForCollision(int targetX, int targetY)
{
    char col = '\0';    //null by default

    // check if it collides with border
    if (targetX < 0 || targetX > pf::b[0].size() - 1 
        || targetY < 0 || targetY > pf::b.size() - 1)
    {
        return 'b';
    }

    char obj = pf::b[targetY][targetX];

    // check if it collides with zombie 
    if (int(obj) > 48 && int(obj) < 58)   // ASCII: 49-57 = 1-9
    {
        for (int i = 0; i < pf::zombiesArray.size(); ++i)
        {   
            if (obj == pf::zombiesArray[i].character)
            {
                return pf::zombiesArray[i].character;
            }
        }
    }

    // check if it collides with collidable objects
    for (int i = 0; i < g_collidableObjects.size(); ++i)
    {
        if (obj == g_collidableObjects[i])
        {
           return g_collidableObjects[i];
        }
    }

    return col;
}

void Alien::GetNextCoord(string direction, int& rTargetX, int& rTargetY)
{
    if (direction == g_up)
    {
        rTargetX = x;
        rTargetY = y-1;
    }
    else if (direction == g_down)
    {
        rTargetX = x;
        rTargetY = y+1;
    }
    else if (direction == g_left)
    {
        rTargetX = x-1;
        rTargetY = y;
    }
    else if (direction == g_right)
    {
        rTargetX = x+1;
        rTargetY = y;
    }
}

void Alien::Move(int targetX, int targetY)
{
    char col = CheckForCollision(targetX, targetY);
    bool canMove = true;

    if (col)
    {
        canMove = Collide(col, targetX, targetY);
    }

    if (canMove)
    {
        pf::b[y][x] = g_trail;
        x = targetX;
        y = targetY;
        pf::b[y][x] = g_alien;
    }
    else
    {
        active = false;     // end turn
    }
    
}

