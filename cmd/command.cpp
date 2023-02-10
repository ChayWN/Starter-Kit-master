#include "../dependencies.h"

namespace cmd
{
    string movingDirection;
    int commandNo = -1;
    bool runningCommand = false;
    int activeZombieIndex;

    vector<string> commandList =
        {
            "up", "down", "left", "right", "fireball", "arrow", "help", "save", "load", "quit"};

    void RefreshBoard()
    {
        pf::ClearScreen();
        pf::ShowGameBoard();
    }

    void ColorString(string str, int color_index)
    {
        std::cout << "\033[1;3" << color_index << "m" << str << "\033[0m";
    }

    void RestartPrompt()
    {
        char again;

        cout << "Play again? (y/n) => ";
        cin >> again;
        again = tolower(again);

        if (again == 'y')
        {
            cout << "\nNew board initialized." << endl;
            pf::CreateGameBoard();
        }
        else if (again == 'n')
        {
            cout << "\nGoodbye!" << endl;
            g_quitGame = true;
        }
        else
        {
            cout << "Invalid Input!" << endl;
            pf::Pause();
            RefreshBoard();
            RestartPrompt();
        }
    }

    void CheckWinner()
    {
        if (pf::alien.isDead)
        {
            pf::Pause();
            RefreshBoard();
            cout << "Zombie wins." << endl;
            RestartPrompt();
        }
        else
        {
            for (int i = 0; i < pf::zombiesArray.size(); ++i)
            {
                Zombie &z = pf::zombiesArray[i];
                if (!z.isDead)
                {
                    return;
                }
            }
            pf::Pause();
            RefreshBoard();
            cout << "Alien wins." << endl;
            RestartPrompt();
        }
    }

    void RockDialogue()
    {
        char objFound = col::underRock;

        if (objFound == g_arrowUp || objFound == g_arrowDown || objFound == g_arrowLeft || objFound == g_arrowRight)
        {
            cout << "Alien discovers an arrow beneath the rock." << endl;
            return;
        }

        switch (objFound)
        {
        case 'h':
            cout << "Alien discovers a health pack beneath the rock." << endl;
            break;
        case 'p':
            cout << "Alien discovers a pod beneath the rock." << endl;
            break;
        case ' ':
            cout << "Alien discovers nothing beneath the rock." << endl;
            break;
        }
    }

    void ZombieDamagedDialogue(Zombie z, int dmg)
    {
        cout << "Zombie " << z.sequence << " receives a damage of " << dmg << "." << endl;

        if (z.isDead)
        {
            cout << "Zombie " << z.sequence << " is defeated." << endl;
        }
        else
        {
            cout << "Zombie " << z.sequence << " is still alive." << endl;
        }
    }

    void AlienDamagedDialogue()
    {
        Zombie &activeZombie = pf::zombiesArray[activeZombieIndex];

        cout << "Zombie " << activeZombie.sequence << " attacks Alien." << endl;
        cout << "Alien receives a damage of " << activeZombie.attack << "." << endl;

        if (pf::alien.isDead)
        {
            cout << "Alien is defeated." << endl
                 << endl;
            CheckWinner();
        }
        else
        {
            cout << "Alien is still alive." << endl
                 << endl;
        }
    }

    void PodDialogue()
    {
        Zombie z = pf::zombiesArray[col::zombieToPodIndex];
        ZombieDamagedDialogue(z, 10);
    }

    void PoisonDialogue()
    {
        Alien &a = pf::alien;

        if (a.poison)
        {
            cout << "Uhoh! Alien got poisoned and receives a damage of 10 for " << a.poison << " round(s)." << endl;

            if (a.isDead)
            {
                cout << "Alien is defeated." << endl
                     << endl;
                CheckWinner();
            }
            else
            {
                cout << "Alien is still alive." << endl;
            }
        }
        else
        {
            cout << "Alien is no longer poisoned." << endl;
        }
    }

    void ZombieWitchDialogue(Zombie z)
    {
        if (z.poison == 3)
        {
            cout << "Zombie " << z.sequence << " receives an additional attack of 10 from the witch for 3 rounds." << endl;
        }
        else if (!z.poison)
        {
            cout << "The witch's spell has worn off. Zombie " << z.sequence << "'s attack has returned to normal." << endl
                 << endl;
            pf::Pause();
            RefreshBoard();
        }
    }

    void AlienDialogue(char col)
    {
        if (col == g_arrowUp || col == g_arrowDown || col == g_arrowLeft || col == g_arrowRight)
        {
            cout << "Alien finds an arrow." << endl;
            cout << "Alien's attack is increased by 20." << endl;
        }
        else if (int(col) > 48 && int(col) < 58)
        {
            for (int i = 0; i < pf::zombiesArray.size(); ++i)
            {
                if (col == pf::zombiesArray[i].character)
                {
                    Zombie &z = pf::zombiesArray[i];
                    cout << "Alien attacks Zombie " << z.sequence << "." << endl;
                    ZombieDamagedDialogue(z, pf::alien.attack);
                    break;
                }
            }
        }
        else
        {
            switch (col)
            {
            case 'b':
                cout << "Alien hits a border." << endl;
                break;
            case 'h':
                cout << "Alien finds a ";
                ColorString("health pack.\n", 2);
                cout << "Alien's life is increased by 20." << endl;
                break;
            case 'r':
                cout << "Alien stumbles upon a ";
                ColorString("rock.\n", 4);
                RockDialogue();
                break;
            case 'p':
                cout << "Alien finds a ";
                ColorString("pod.\n", 3);
                PodDialogue();
                break;
            case 'w':
                cout << "Alien finds a ";
                ColorString("witch.\n", 5);
                PoisonDialogue();
                break;
            case '\2':
                break;
            case 'o':
                cout << "Alien finds a ";
                ColorString("fireball.\n", 1);
                cout << "Alien has " << pf::alien.fireball << " fireball(s). " << endl;
                break;
            default:
                cout << "Alien finds an empty space." << endl;
            }
        }
        cout << endl;
    }

    void ZombieTurnEnd()
    {
        RefreshBoard();

        Zombie &activeZombie = pf::zombiesArray[activeZombieIndex];

        cout << "Zombie " << activeZombie.sequence << "'s turn ends." << endl
             << endl;

        pf::Pause();

        activeZombie.active = false;

        int oldIndex = activeZombieIndex;

        // switch to next zombie
        for (int i = activeZombieIndex + 1; i < pf::zombiesArray.size(); ++i)
        {
            Zombie &z = pf::zombiesArray[i];
            if (!z.isDead)
            {
                z.active = true;
                activeZombieIndex = i;
                break;
            }
        }

        // if could not find a next zombie, switch back to alien
        if (activeZombieIndex == oldIndex)
        {
            pf::alien.active = true;
            if (pf::alien.poison)
            {
                pf::alien.triggerPoison = true;
            }
        }

        CheckWinner();
    }

    void ZombieTurn()
    {
        Zombie &activeZombie = pf::zombiesArray[activeZombieIndex];

        int targetX, targetY;
        string direction = activeZombie.GetDirection(targetX, targetY);

        if (activeZombie.poison)
        {
            col::ZombieWitchEncounter(activeZombie);
            ZombieWitchDialogue(activeZombie);
        }

        activeZombie.Move(targetX, targetY);
        cout << "Zombie " << activeZombie.sequence << " moves " << direction << "." << endl;

        if (activeZombie.encounterWitch)
        {
            cout << "Zombie encounters a witch." << endl;
            col::ZombieWitchEncounter(activeZombie);
            ZombieWitchDialogue(activeZombie);
            activeZombie.encounterWitch = false;
        }
        cout << endl;

        pf::Pause();
        RefreshBoard();

        // if alien is within range of zombie, attack alien
        bool canAttack = activeZombie.CanAttack(pf::alien);

        if (canAttack)
        {
            activeZombie.Attack(pf::alien);
            AlienDamagedDialogue();
            if (g_quitGame)
            {
                return;
            }
        }
        else
        {
            cout << "Zombie " << activeZombie.sequence << " is unable to attack Alien." << endl;
            cout << "Alien is too far. " << endl
                 << endl;
        }

        pf::Pause();

        ZombieTurnEnd();
    }

    void AlienTurnEnd()
    {
        // Toggling aline.active to show the '->' symbol beside alien for one last time
        pf::alien.active = true;
        RefreshBoard();

        cout << "Alien's turn ends. The trail is reset." << endl
             << endl;

        pf::alien.active = false;

        // switch turn to zombie
        for (int i = 0; i < pf::zombiesArray.size(); ++i)
        {
            Zombie &z = pf::zombiesArray[i];

            if (!z.isDead)
            {
                z.active = true;
                activeZombieIndex = i;
                break;
            }
        }

        pf::RefillGameObjects();
        pf::alien.ResetAttack();

        CheckWinner();
        pf::Pause();
    }

    void AlienMove()
    {
        RefreshBoard();

        int targetX, targetY;
        pf::alien.GetNextCoord(movingDirection, targetX, targetY);

        if (pf::alien.poison && pf::alien.triggerPoison)
        {
            RefreshBoard();
            col::AlienWitchEncounter();
            PoisonDialogue();
            cout << endl;
            pf::alien.triggerPoison = false;

            pf::Pause();
            RefreshBoard();
        }

        char col = pf::alien.CheckForCollision(targetX, targetY);

        pf::alien.Move(targetX, targetY);

        AlienDialogue(col);

        pf::Pause();

        if (pf::alien.active)
        {
            AlienMove();
        }
        else
        {
            AlienTurnEnd();
        }
    }

    void ShootFireball(int targetX, int targetY)
    {
        int fireballDmg = 5;
        vector<vector<int>> targetedCoords;
        vector<vector<int>> explosionCoords;

        vector<int> center = {targetX, targetY};
        targetedCoords.push_back(center);
        vector<int> left = {targetX - 1, targetY};
        targetedCoords.push_back(left);
        vector<int> right = {targetX + 1, targetY};
        targetedCoords.push_back(right);
        vector<int> top = {targetX, targetY - 1};
        targetedCoords.push_back(top);
        vector<int> topLeft = {targetX - 1, targetY - 1};
        targetedCoords.push_back(topLeft);
        vector<int> topRight = {targetX + 1, targetY - 1};
        targetedCoords.push_back(topRight);
        vector<int> bottom = {targetX, targetY + 1};
        targetedCoords.push_back(bottom);
        vector<int> bottomLeft = {targetX - 1, targetY + 1};
        targetedCoords.push_back(bottomLeft);
        vector<int> bottomRight = {targetX + 1, targetY + 1};
        targetedCoords.push_back(bottomRight);

        for (int i = 0; i < targetedCoords.size(); ++i)
        {
            vector<int> coord = targetedCoords[i];
            int x = coord[0], y = coord[1];

            if (x < 0 || y < 0 || y > pf::kRows - 1 || x > pf::kColumns - 1)
            {
                continue;
            }
            else
            {
                explosionCoords.push_back(coord);
            }
        }

        cout << "\nThe fireball has destroyed nearby objects. " << endl;
        for (int i = 0; i < explosionCoords.size(); ++i)
        {
            vector<int> coord = explosionCoords[i];
            char obj = pf::b[coord[1]][coord[0]];

            if (obj == g_alien)
            {
                continue;
            }
            else if (int(obj) > 48 && int(obj) < 58) // ASCII: 49-57 = 1-9
            {
                for (int j = 0; j < pf::zombiesArray.size(); ++j)
                {
                    if (obj == pf::zombiesArray[j].character)
                    {
                        Zombie &z = pf::zombiesArray[j];
                        cout << "Zombie " << z.sequence << " is within the explosion range." << endl;
                        z.TakeDamage(fireballDmg);
                        ZombieDamagedDialogue(z, fireballDmg);
                    }
                }
            }
            else
            {
                pf::b[coord[1]][coord[0]] = g_trail;
            }
        }
        cout << endl;
        pf::Pause();
        RefreshBoard();
        pf::alien.DecreaseFireball();
        cout << "Alien has " << pf::alien.fireball << " fireball(s) left." << endl
             << endl;
    }

    void FireballCommand()
    {
        if (!pf::alien.fireball)
        {
            cout << "Alien do not have any fireball to shoot." << endl
                 << endl;
            pf::Pause();
            return;
        }
        int targetX, targetY;
        cout << "Enter row and column to shoot fireball: ";
        cin >> targetY >> targetX;

        targetX--;
        targetY--;

        if (targetX < 0 || targetY < 0 || targetY > pf::kRows - 1 || targetX > pf::kColumns - 1)
        {
            cout << "Invalid input. Rows and columns are out of bound of the board." << endl;
            pf::Pause();
            return;
        }
        else
        {
            ShootFireball(targetX, targetY);
            pf::Pause();
            CheckWinner();
        }
    }

    void ArrowSwitch(char &oldArrow, char newArrow)
    {
        cout << "Arrow " << oldArrow << " is switched to " << newArrow << endl;
        oldArrow = newArrow;
    }

    void ArrowMovement()
    {
        string arr[3];
        cout << "Enter row, column and direction: ";
        for (int i = 0; i < 3; i++)
        {
            cin >> arr[i];
        }

        int arow = stoi(arr[0]) - 1;
        int acol = stoi(arr[1]) - 1;
        string adirection = arr[2];

        if (arow < 0 || acol < 0)
        {
            cout << "Invalid input. Please enter positive integer number for rows and columns." << endl;
            pf::Pause();
            return;
        }
        else if (arow > pf::kRows - 1 || acol > pf::kColumns - 1)
        {
            cout << "Invalid input. Rows and columns are out of bound of the board." << endl;
            pf::Pause();
            return;
        }

        if (pf::b[arow][acol] == g_arrowUp || pf::b[arow][acol] == g_arrowDown || pf::b[arow][acol] == g_arrowLeft || pf::b[arow][acol] == g_arrowRight)
        {
            char &oldArrow = pf::b[arow][acol];

            if (adirection == g_up)
            {
                ArrowSwitch(oldArrow, g_arrowUp);
            }
            else if (adirection == g_down)
            {
                ArrowSwitch(oldArrow, g_arrowDown);
            }
            else if (adirection == g_left)
            {
                ArrowSwitch(oldArrow, g_arrowLeft);
            }
            else if (adirection == g_right)
            {
                ArrowSwitch(oldArrow, g_arrowRight);
            }
            else
            {
                cout << "Invalid direction. Please enter the correct direction ( up / down / left/ right )" << endl;
            }
        }
        else
        {
            cout << "Invalid input. The row and column that you entered does not contain an arrow." << endl;
        }
        pf::Pause();
    }

    void SaveGame()
    {
        string fileName;
        cout << "Enter the file name to save the current game: ";
        cin >> fileName;

        ofstream fileOut(fileName);

        if (fileOut.is_open())
        {
            // Store board dimensions and characters
            fileOut << pf::kRows << " " << pf::kColumns << "\n";
            for (int i = 0; i < pf::kRows; ++i)
            {
                for (int j = 0; j < pf::kColumns; ++j)
                {
                    fileOut << pf::b[i][j];
                }
            }
            fileOut << "\n";

            // Store alien stats
            Alien a = pf::alien;
            fileOut << a.x << " " << a.y << " " << a.maxLife << " " << a.life << " " << a.attack
                    << " " << a.poison << " " << a.triggerPoison << " " << a.fireball << "\n";

            // Store zombie stats
            for (int i = 0; i < pf::zombiesArray.size(); ++i)
            {
                Zombie z = pf::zombiesArray[i];
                fileOut << z.x << " " << z.y << " " << z.sequence << " " << z.life << " " << z.attack
                        << " " << z.range << " " << z.initialLife << " " << z.initialAttack << " " << z.initialRange
                        << " " << z.poison << " " << z.encounterWitch << "\n";
            }

            fileOut.close();

            cout << "Game Saved" << endl;
        }
        else
        {
            cout << "Error: Unable to open file" << endl;
        }
        pf::Pause();
    }

    vector<int> BreakDownLine(string line)
    {
        // Used to break down words in 'line' by space
        istringstream ss(line);

        // Store individual number
        int num;
        vector<int> v;

        while (ss >> num)
        {
            v.push_back(num);
        }

        return v;
    }

    vector<vector<char>> LoadGameBoard(string line, int nRows, int nColumns)
    {
        vector<vector<char>> b;
        int lineIndex = 0;

        b.resize(nRows);
        for (int i = 0; i < nRows; ++i)
        {
            b[i].resize(nColumns);
            for (int j = 0; j < nColumns; ++j)
            {
                b[i][j] = line[lineIndex];

                lineIndex++;
            }
        }

        return b;
    }

    void LoadAlienStats(string line)
    {
        vector<int> AStats = BreakDownLine(line);

        pf::alien.x = AStats[0];
        pf::alien.y = AStats[1];
        pf::alien.maxLife = AStats[2];
        pf::alien.life = AStats[3];
        pf::alien.attack = AStats[4];
        pf::alien.poison = AStats[5];
        pf::alien.triggerPoison = AStats[6];
        pf::alien.fireball = AStats[7];
    }

    void LoadZombieStats(string line, vector<Zombie> &ZArray)
    {
        vector<int> ZStats = BreakDownLine(line);

        if (ZStats.size() < 11)
            return;

        Zombie z(ZStats[2], ZStats[0], ZStats[1]); // Zombie(sequence no, x, y)

        z.life = ZStats[3];
        z.attack = ZStats[4];
        z.range = ZStats[5];
        z.initialLife = ZStats[6];
        z.initialAttack = ZStats[7];
        z.initialRange = ZStats[8];
        z.poison = ZStats[9];
        z.encounterWitch = ZStats[10];

        ZArray.push_back(z);
    }

    void LoadGame()
    {
        char input;
        cout << "Do you want to save the current game? (y/n): ";
        cin >> input;
        input = tolower(input);

        if (input == 'y')
        {
            SaveGame();
            RefreshBoard();
        }
        else if (input != 'n')
        {
            cout << "Invalid Input!" << endl;
            pf::Pause();
            return;
        }

        string fileName;
        cout << "Enter the file name to load: ";
        cin >> fileName;

        ifstream fileIn(fileName);

        if (fileIn.is_open())
        {
            string line;
            int nRows, nColumns;
            vector<vector<char>> newBoard;
            vector<Zombie> newZombieArray;

            for (int ln = 1; getline(fileIn, line); ln++)
            {
                switch (ln)
                {
                case 1:
                {
                    vector<int> boardDimension = BreakDownLine(line);
                    nRows = boardDimension[0];
                    nColumns = boardDimension[1];
                    break;
                }
                case 2:
                    newBoard = LoadGameBoard(line, nRows, nColumns);
                    break;
                case 3:
                    LoadAlienStats(line);
                    break;
                default:
                    LoadZombieStats(line, newZombieArray);
                    break;
                }
            }

            pf::kRows = nRows;
            pf::kColumns = nColumns;
            pf::b = newBoard;
            pf::zombiesArray = newZombieArray;
            pf::kZombies = newZombieArray.size();

            fileIn.close();

            cout << "Game Loaded" << endl;
        }
        else
        {
            cout << "Error: File does not exist" << endl;
        }
        pf::Pause();
    }

    void Quit()
    {
        char input;
        cout << "Are you sure? (y/n): ";
        cin >> input;
        input = tolower(input);
        cout << endl;

        if (input == 'y')
        {
            cout << "Goodbye!" << endl;
            pf::Pause();
            g_quitGame = true;
        }
        else if (input == 'n')
        {
            cout << "You have cancelled quit." << endl;
            pf::Pause();
        }
        else
        {
            cout << "Invalid Input!" << endl;
            pf::Pause();
        }
    }

    void Help()
    {
        cout << "Commands" << endl;
        cout << "1. up       -"
             << "Move up." << endl;
        cout << "2. down     -"
             << "Move down." << endl;
        cout << "3. left     -"
             << "Move left." << endl;
        cout << "4. right    -"
             << "Move right." << endl;
        cout << "5. fireball -"
             << "Deal explosion with fireball." << endl;
        cout << "6. arrow    -"
             << "Change the direction of an arrow." << endl;
        cout << "7. help     -"
             << "Display these user commands." << endl;
        cout << "8. save     -"
             << "Save the game." << endl;
        cout << "9. load     -"
             << "Load a game." << endl;
        cout << "10. quit    -"
             << "Quit the game." << endl;

        pf::Pause();
    }

    void RunCommand(int commandNo)
    {
        switch (commandNo)
        {
        case 0:
            movingDirection = g_up;
            AlienMove();
            break;
        case 1:
            movingDirection = g_down;
            AlienMove();
            break;
        case 2:
            movingDirection = g_left;
            AlienMove();
            break;
        case 3:
            movingDirection = g_right;
            AlienMove();
            break;
        case 4:
            FireballCommand();
            break;
        case 5:
            ArrowMovement();
            break;
        case 6:
            Help();
            break;
        case 7:
            SaveGame();
            break;
        case 8:
            LoadGame();
            break;
        case 9:
            Quit();
            break;
        default:
            cout << "Command not implemented yet" << endl
                 << endl;
            pf::Pause();
        }

        runningCommand = false;
        commandNo = -1;
    }

    void CommandInput()
    {
        string command;

        cout << "command> ";

        if (runningCommand)
        {
            RunCommand(commandNo);
            return;
        }

        cin >> command;
        cout << endl;

        for (int i = 0; i < commandList.size(); ++i)
        {
            if (command == commandList[i])
            {
                commandNo = i;
                break;
            }
            else
            {
                commandNo = -1;
            }
        }

        if (commandNo == -1)
        {
            cout << "Invalid Command! Type \"help\" for the list of user commands." << endl;
            pf::Pause();
        }
        else
        {
            runningCommand = true;
            RunCommand(commandNo);
        }
    }
}