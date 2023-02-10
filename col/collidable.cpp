#include "../dependencies.h"

namespace col
{
    char underRock;
    int zombieToPodIndex;
    int witchStat = 10;
    bool canPoisonAlien;

    void HealthPickUp()
    {
        pf::alien.Heal(20);
    }

    void RockEncounter(int x, int y)
    {
        underRock = pf::RandomObject();
        
        if (underRock == g_rock)
        {
            underRock = g_empty;
        }
        pf::b[y][x] = underRock;
    }

    void PodPickUp()
    {
        int podX, podY;
        vector<int> arr;
        int smallestIndex;
        pf::alien.GetNextCoord(cmd::movingDirection, podX, podY);

        for (int i = 0; i < pf::zombiesArray.size(); ++i)
        {
            Zombie &z = pf::zombiesArray[i];

            int diffY = abs(podY - z.y);
            int diffX = abs(podX - z.x);
            int distance = diffY + diffX;
            arr.push_back(distance);            
        } 

        for (int i = 0; i < pf::zombiesArray.size(); ++i)
        {
            if (!pf::zombiesArray[i].isDead)
            {
                smallestIndex = i;
            }      
        }
        
        for (int i = 0; i < arr.size(); ++i)
        {
            if (arr[i] < arr[smallestIndex] && !pf::zombiesArray[i].isDead)
            {
                smallestIndex = i;
            }
        }

        zombieToPodIndex = smallestIndex;
        pf::zombiesArray[smallestIndex].TakeDamage(10);
    }

    void ArrowPickUp(string direction)
    {
        cmd::movingDirection = direction;
        pf::alien.IncreaseAttack(20);
    }

    void FireballPickUp()
    {
        pf::alien.IncreaseFireball();
    }

    void AlienWitchEncounter()
    {
        Alien& a = pf::alien;
        
        a.poison--;
        
        if (a.poison == 0)
        {
            a.triggerPoison = false;
        }
        else
        {
            a.TakeDamage(witchStat);
        }
    }

    void PoisonConsume()
    {
        Alien& a = pf::alien;

        a.poison = 4;
        AlienWitchEncounter();

    }

    void ZombieWitchEncounter(Zombie& z)
    {     
        if (!z.poison || z.encounterWitch)
        {
            z.poison = 3;
            z.ResetAttack();
            z.IncreaseAttack(witchStat);
        }  
        else if (z.poison)
        {
            z.poison--;

            if (z.poison == 0)
            {
                z.ResetAttack();
            }
        }
        
    }

    void OnCollide(char col, int x, int y)
    {
        switch (col)
        {
        case 'h':
            HealthPickUp();
            break;
        case 'r' :
            RockEncounter(x, y);
            break;
        case 'p':
            PodPickUp();
            break;
        case '^':
            ArrowPickUp(g_up);
            break;
        case 'v':
            ArrowPickUp(g_down);
            break;
        case '<':
            ArrowPickUp(g_left);
            break;
        case '>':
            ArrowPickUp(g_right);
            break;
        case 'w':
            PoisonConsume();
            break;
        case 'o':
            FireballPickUp();
            break;
        }
    }
}