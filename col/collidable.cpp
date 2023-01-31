#include "../dependencies.h"

namespace col
{
    char underRock;
    int zombieToPodIndex;

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
        int smallest;
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
                smallest = arr[i];
            }      
        }
        
        for (int i = 0; i < arr.size(); ++i)
        {
            if (arr[i] < smallest && !pf::zombiesArray[i].isDead)
            {
                smallest = arr[i];
            }
        }

        for (int i = 0; i < arr.size(); ++i)
        {
            if (arr[i] == smallest)
            {
                zombieToPodIndex = i;
                pf::zombiesArray[i].TakeDamage(10);
                break;
            }
        }
    }

    void ArrowPickUp(string direction)
    {
        cmd::movingDirection = direction;
        pf::alien.IncreaseAttack(20);
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
        }
    }
}