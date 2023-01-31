#ifndef CMD_COMMAND_H
#define CMD_COMMAND_H

#include <string>

namespace cmd
{
    extern std::string movingDirection;

    void CommandInput();
    void ZombieTurn();
}

#endif