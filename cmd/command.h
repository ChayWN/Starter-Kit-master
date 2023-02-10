#ifndef CMD_COMMAND_H
#define CMD_COMMAND_H

#include <string>

namespace cmd
{
    extern std::string movingDirection;

    void RefreshBoard();
    void CommandInput();
    void ZombieTurn();
    void ZombieDamagedDialogue(Zombie z, int dmg);
}

#endif