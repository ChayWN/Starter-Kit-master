#include "../dependencies.h"

namespace cmd
{
    string movingDirection;
    int commandNo = -1;
    bool runningCommand = false;
    int activeZombieIndex;

    vector<string> commandList =
        {
            "up", "down", "left", "right", "arrow", "help", "save", "load", "quit"};

    void RefreshBoard()
    {
        pf::ClearScreen();
        pf::ShowGameBoard();
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
            cout << "Alien is defeated." << endl;
        }
        else
        {
            cout << "Alien is still alive." << endl;
        }
        cout << endl;
    }

    void PodDialogue()
    {
        Zombie z = pf::zombiesArray[col::zombieToPodIndex];
        ZombieDamagedDialogue(z, 10);
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
                cout << "Alien finds a health pack." << endl;
                cout << "Alien's life is increased by 20." << endl;
                break;
            case 'r':
                cout << "Alien stumbles upon a rock." << endl;
                RockDialogue();
                break;
            case 'p':
                cout << "Alien finds a pod." << endl;
                PodDialogue();
                break;
            default:
                cout << "Alien finds an empty space." << endl;
            }
        }
        cout << endl;
    }

    void RestartPrompt()
    {
        char again;

        cout << "Play again? (y/n)> ";
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
        }

        CheckWinner();
    }

    void ZombieTurn()
    {
        Zombie &activeZombie = pf::zombiesArray[activeZombieIndex];

        int targetX, targetY;
        string direction = activeZombie.GetDirection(targetX, targetY);
        
        activeZombie.Move(targetX, targetY);
        cout << "Zombie " << activeZombie.sequence << " moves " << direction << "." << endl 
            << endl;

        pf::Pause();

        RefreshBoard();

        bool canAttack = activeZombie.CanAttack(pf::alien);

        // if alien is within range of zombie, attack alien
        if (canAttack)
        {
            activeZombie.Attack(pf::alien);
            AlienDamagedDialogue();
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
        else if (arow > pf::kRows || acol > pf::kColumns){
            cout << "Invalid input. Rows and columns are out of bound for the table." << endl;
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

    void SaveGame(){
        string fileName;
        cout << "Enter the file name to save the current game: ";
        cin >> fileName;

        ofstream fileOut(fileName);

        if (fileOut.is_open())
        {
            // Store board dimensions and characters
            fileOut << pf::kRows << " " << pf::kColumns << "\n";
            for (int i=0; i<pf::kRows; ++i)
            {
                for (int j=0; j<pf::kColumns;++j)
                {
                    fileOut << pf::b[i][j];
                }
            }
            fileOut << "\n";

            // Store alien stats
            Alien a = pf::alien;
            fileOut << a.x << " " << a.y << " " << a.maxLife << " " << a.life << " " << a.attack << "\n";

            // Store zombie stats
            for (int i=0; i<pf::zombiesArray.size(); ++i)
            {
                Zombie z = pf::zombiesArray[i];
                fileOut << z.x << " " << z.y << " " << z.sequence << " " << z.life << " " << z.attack 
                    << " " << z.range << " " << z.initialLife << " " << z.initialAttack << " " << z.initialRange << "\n";
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
    }

    void LoadZombieStats(string line, vector<Zombie>& ZArray)
    {
        vector<int> ZStats = BreakDownLine(line);
        
        if (ZStats.size() < 6) return;

        Zombie z(ZStats[2], ZStats[0], ZStats[1]); // Zombie(sequence no, x, y)

        z.life = ZStats[3];
        z.attack = ZStats[4]; 
        z.range = ZStats[5]; 
        z.initialLife = ZStats[6]; 
        z.initialAttack = ZStats[7]; 
        z.initialRange = ZStats[8]; 

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

            for (int ln=1; getline(fileIn, line); ln++)
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
                default:
                    LoadZombieStats(line, newZombieArray);
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
        cout << "1. up      -"
             << "Move up." << endl;
        cout << "2. down    -"
             << "Move down." << endl;
        cout << "3. left    -"
             << "Move left." << endl;
        cout << "4. right   -"
             << "Move right." << endl;
        cout << "5. arrow   -"
             << "Change the direction of an arrow." << endl;
        cout << "6. help    -"
             << "Display these user commands." << endl;
        cout << "7. save    -"
             << "Save the game." << endl;
        cout << "8. load    -"
             << "Load a game." << endl;
        cout << "9. quit    -"
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
            ArrowMovement();
            break;
        case 5:
            Help();
            break;
        case 6:
            SaveGame();
            break;
        case 7:
            LoadGame();
            break;
        case 8:
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