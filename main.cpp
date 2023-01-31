// ********************************************************* 
// Course: TCP1101 PROGRAMMING FUNDAMENTALS 
// Year: Trimester 1, 2022/23 (T2215) 
// Lab: TT7L 
// Names: Chay Wen Ning | Melody Koh Si Jie
// IDs: 1201103431 | 1201103207
// Emails: 1201103431@student.mmu.edu.my | 1201103207@student.mmu.edu.my
// Phones: 016-2855632 | 019-3426968
// ********************************************************* 

#include "dependencies.h"

const char g_alien = 'A'; 
const char g_arrowUp = '^';
const char g_arrowDown = 'v';
const char g_arrowLeft = '<';
const char g_arrowRight = '>';
const char g_health = 'h';
const char g_pod = 'p';
const char g_rock = 'r';
const char g_empty = ' ';
const char g_trail = '.';

vector<char> g_collidableObjects = 
{
    g_arrowUp, g_arrowDown, g_arrowLeft, g_arrowRight,
    g_health, g_pod, g_rock
};

const string g_up = "up";
const string g_down = "down";
const string g_left = "left";
const string g_right = "right";

bool g_quitGame = false;

int g_currentState = 0;

void Game()
{
    pf::ClearScreen();
    pf::ShowGameBoard();
    pf::CarryOutTurns();

}

void GameStart()
{
    pf::DefaultSettings();
    pf::CreateGameBoard();

    g_currentState++;
}

void Update(int state)
{
    switch (state)
    {
    case 0:
        GameStart();
    case 1:
        Game();
    }
}

int main()
{
    srand(time(NULL));

    cout << "Assignment (Part 1)" << endl;
    cout << "Let's Get Started!" << endl;
    pf::Pause();
    
    while (!g_quitGame)
    {   
        Update(g_currentState);
    }
}