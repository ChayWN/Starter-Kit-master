#include "../dependencies.h"

using namespace std;

typedef vector<vector<char>> board;

namespace pf
{
    int kRows = 3;
    int kColumns = 5;
    int kZombies = 1;

    bool alienTurns = true;
    bool firstInitialization = true;

    board b;
    Alien alien;
    vector<Zombie> zombiesArray;

    int ClearScreen()
    {
#if defined(_WIN32)
        return system("cls");
#elif defined(__linux__) || defined(__APPLE__)
        return system("clear");
#endif
    }

    int Pause()
    {
#if defined(_WIN32)
        return system("pause");
#elif defined(__linux__) || defined(__APPLE__)
        return system(R"(read -p "Press any key to continue . . . " dummy)");
#endif
    }

    void CarryOutTurns()
    {
        if (alien.active)
        {
            cmd::CommandInput();
        }
        else
        {
            cmd::ZombieTurn();
        };
    }

    char RandomObject(int random_number /*= rand()*/)
    {
        vector<char> objects =
            {
                g_empty, g_empty, g_empty, g_empty, g_empty,
                g_rock, g_rock, g_pod, g_pod, g_health, g_health,
                g_arrowUp, g_arrowDown, g_arrowLeft, g_arrowRight};

        char randomObj = objects[random_number % objects.size()];

        return randomObj;
    }

    void RefillGameObjects()
    {
        for (int row = 0; row < kRows; ++row)
        {
            for (int col = 0; col < kColumns; ++col)
            {
                if (b[row][col] == g_trail)
                {
                    b[row][col] = RandomObject();
                }
            }
        }
    }

    void SpawnGameObjects()
    {
        int zombieCount;
        int alienY = (kRows - 1) / 2, alienX = (kColumns - 1) / 2;

        alien.x = alienX;
        alien.y = alienY;
        b[alienY][alienX] = g_alien;

        // reset alien's values
        alien.Reset();

        do
        {
            zombieCount = 0;
            for (int row = 0; row < kRows; ++row)
            {
                for (int col = 0; col < kColumns; ++col)
                {
                    if (b[row][col] == g_alien)
                        continue;

                    int random_number = rand();

                    if (zombieCount < kZombies && random_number % (kColumns * kRows / kZombies + 1) == 0)
                    {
                        if (firstInitialization)
                        {
                            Zombie z(zombieCount + 1, col, row);
                            b[row][col] = z.character;
                            zombiesArray.push_back(z);
                        }
                        else
                        {
                            Zombie &z = zombiesArray[zombieCount];
                            b[row][col] = z.character;
                            z.Reset();
                        }
                        zombieCount++;
                    }
                    else
                    {
                        b[row][col] = RandomObject(random_number);
                    }
                }
            }
        } while (zombieCount < kZombies);

        firstInitialization = false;
    }

    void CreateGameBoard()
    {
        b.resize(kRows);
        for (int row = 0; row < kRows; ++row)
        {
            b[row].resize(kColumns);

            for (int col = 0; col < kColumns; ++col)
            {
                b[row][col] = g_empty;
            }
        }
        SpawnGameObjects();
    }

    void DisplayCharacters()
    {
        if (alien.active)
        {
            cout << "->  ";
        }
        else
        {
            cout << "    ";
        }
        cout << "Alien     : Life " << alien.life << ", Attack  " << alien.attack << endl;
        for (int i = 0; i < zombiesArray.size(); ++i)
        {
            Zombie z = zombiesArray[i];
            if (z.active)
            {
                cout << "->  ";
            }
            else
            {
                cout << "    ";
            }
            cout << "Zombie " << z.sequence << "  : Life " << z.life << ", Attack  " << z.attack << ", Range  " << z.range << endl;
        };
        cout << endl;
    }

    void ShowGameBoard()
    {
        cout << "\t.: Alien vs Zombie :." << endl;
        for (int row = 0; row < kRows; ++row)
        {
            cout << "    ";
            for (int i = 0; i < kColumns; ++i)
            {
                cout << "+-";
            }
            cout << "+" << endl
                 << "  " << row + 1 << " ";
            for (int col = 0; col < kColumns; ++col)
            {
                cout << "|" << b[row][col];
            }
            cout << "|" << endl;
        }
        cout << "    ";
        for (int j = 0; j < kColumns; ++j)
        {
            cout << "+-";
        }
        cout << "+" << endl;
        cout << "    ";
        for (int j = 0; j < kColumns; ++j)
        {
            int digit = (j + 1) / 10;
            cout << " ";
            if (digit == 0)
                cout << " ";
            else
                cout << digit;
        }
        cout << endl;
        cout << "    ";
        for (int j = 0; j < kColumns; ++j)
        {
            cout << " " << (j + 1) % 10;
        }
        cout << endl
             << endl;
        DisplayCharacters();
    }

    void ChangeSettings()
    {
        ClearScreen();
        cout << "Board Settings" << endl;
        cout << "----------------" << endl;
        cout << "Enter rows    => ";
        cin >> kRows;
        cout << "Enter columns => ";
        cin >> kColumns;

        cout << "\nZombie Settings" << endl;
        cout << "----------------" << endl;
        cout << "Enter number of zombies  => ";
        cin >> kZombies;
        cout << endl;

        if (kRows % 2 == 0 || kColumns % 2 == 0)
        {
            cout << "Number of rows and columns must be odd number!" << endl;
        }
        else if (kRows < 3 || kColumns < 5)
        {
            cout << "Number of rows and columns must have a minimum of 3x5 dimension!" << endl;
        }
        else if (kZombies > 9)
        {
            cout << "Number of zombies cannot be more than 9!" << endl;
        }
        else
        {
            cout << "Settings Updated." << endl;
            Pause();
            return;
        }
        Pause();
        ChangeSettings();
    }

    void DefaultSettings()
    {
        ClearScreen();
        char reply;
        cout << "Default Game Settings" << endl;
        cout << "------------------------" << endl;
        cout << "Board Rows    : " << kRows << endl;
        cout << "Board Columns : " << kColumns << endl;
        cout << "Zombie Count  : " << kZombies << endl
             << endl;
        cout << "Do you wish to change the game settings (y/n)? => ";
        cin >> reply;
        cout << endl;
        reply = tolower(reply);

        if (reply == 'n')
        {
            cout << "Default Settings Maintained." << endl;
            Pause();
            return;
        }
        else if (reply == 'y')
        {
            ChangeSettings();
        }
        else
        {
            cout << "Invalid Input." << endl;
            Pause();
            DefaultSettings();
        }
    }
}