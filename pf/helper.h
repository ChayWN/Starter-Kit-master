#ifndef PF_HELPER_H
#define PF_HELPER_H

#include <vector>
#include <cstdlib>
#include "../alien/alien.h"
#include "../zombie/zombie.h"

typedef std::vector< std::vector<char> > board;

namespace pf
{
    extern board b;
    extern Alien alien;
    extern std::vector<Zombie> zombiesArray;
    extern int kZombies;
    extern int kRows;
    extern int kColumns;

    int ClearScreen();
    int Pause();

    void CarryOutTurns();
    void RefillGameObjects();
    char RandomObject(int random_number = std::rand());
    void CreateGameBoard();
    void ShowGameBoard();
    void DefaultSettings();
}

#endif