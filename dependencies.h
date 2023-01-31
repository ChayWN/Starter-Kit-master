#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

#include "pf/helper.h"
#include "zombie/zombie.h"
#include "col/collidable.h"
#include "alien/alien.h"
#include "cmd/command.h"

using namespace std;

// NOTE: To compile the game, use
//    g++ pf\*.cpp zombie\*.cpp col\*.cpp alien\*.cpp cmd\*.cpp  main.cpp


// global variables (declared in main.cpp)
extern const char g_alien;
extern const char g_arrowUp;
extern const char g_arrowDown;
extern const char g_arrowLeft;
extern const char g_arrowRight;
extern const char g_health;
extern const char g_pod;
extern const char g_rock;
extern const char g_empty;
extern const char g_trail;

extern vector<char> g_collidableObjects;

extern const string g_up;
extern const string g_down;
extern const string g_left;
extern const string g_right;

extern int g_currentState;
extern bool g_quitGame;

#endif