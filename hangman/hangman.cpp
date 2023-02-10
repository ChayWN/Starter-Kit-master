#include "../dependencies.h"

namespace hangman
{
    int hangmanStat = 10;
    bool playHangman = false;

    void FillWordWithCharGivenPositions(const vector<int> &positions, char charToFill,
                                        string &word)
    {
        int numOfPositions = positions.size();
        for (int i = 0; i < numOfPositions; ++i)
        {
            word[positions[i]] = charToFill;
        }
    }

    // function to display all numbers in container v
    void DisplayNumbersInVector(const vector<int> &v)
    {
        int size = v.size();
        for (int i = 0; i < size; ++i)
        {
            cout << v[i] << " ";
        }
    }

    void GetPositionsOfCharInWord(const string &secretWord, char charToCheck,
                                  vector<int> &positions)
    {
        positions.clear();
        for (int i = 0; i < secretWord.length(); ++i)
        {
            if (secretWord[i] == charToCheck)
            {
                positions.push_back(i);
            }
        }
    }

    void ColorString(string str, int color_index)
    {
        std::cout << "\033[1;3" << color_index << "m" << str << "\033[0m" << endl;
    }

    void DisplayHangman(int stage)
    {
        switch (stage)
        {
        case 7:
            ColorString(" +----+     ", 1);
            ColorString(" |/   |     ", 1);
            ColorString(" |    o     ", 1);
            ColorString(" |   /|\\   ", 1);
            ColorString(" |    |     ", 1);
            ColorString(" |   / \\   ", 1);
            ColorString(" |  R.I.P.  ", 1);
            ColorString(" ========== ", 1);
            break;

        case 6:
            ColorString(" +----+     ", 1);
            ColorString(" |/   |     ", 1);
            ColorString(" |    o     ", 1);
            ColorString(" |   /|\\   ", 1);
            ColorString(" |    |     ", 1);
            ColorString(" |   /      ", 1);
            ColorString(" |          ", 1);
            ColorString(" ========== ", 1);
            break;
        case 5:
            ColorString(" +----+     ", 1);
            ColorString(" |/   |     ", 1);
            ColorString(" |    o     ", 1);
            ColorString(" |   /|\\   ", 1);
            ColorString(" |    |     ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" ========== ", 1);
            break;
        case 4:
            ColorString(" +----+     ", 1);
            ColorString(" |/   |     ", 1);
            ColorString(" |    o     ", 1);
            ColorString(" |   /|\\   ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" ========== ", 1);
            break;
        case 3:
            ColorString(" +----+     ", 1);
            ColorString(" |/   |     ", 1);
            ColorString(" |    o     ", 1);
            ColorString(" |   /|     ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" ========== ", 1);
            break;
        case 2:
            ColorString(" +----+     ", 1);
            ColorString(" |/   |     ", 1);
            ColorString(" |    o     ", 1);
            ColorString(" |    |     ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" ========== ", 1);
            break;
        case 1:
            ColorString(" +----+     ", 1);
            ColorString(" |/   |     ", 1);
            ColorString(" |    o     ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" ========== ", 1);
            break;
        case 0:
            ColorString(" +----+     ", 1);
            ColorString(" |/   |     ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" |          ", 1);
            ColorString(" ========== ", 1);
            break;
        }
    }

    void HangmanDialogue()
    {
        char input;
        cout << "Alien finds ";
        ColorString("Hangman.\n", 5);
        cout << "Would you like to play Hangman? (y/n) => ";
        cin >> input;
        input = tolower(input);

        switch (input)
        {
        case 'y':
            cout << "\nGuess the correct word for a reward!\n"
                 << endl;
            playHangman = true;
            pf::Pause();
            break;
        case 'n':
            cout << "\nMaybe another day... Hangman leaves." << endl;
            playHangman = false;
            break;
        default:
            cout << "\nInvalid Input! Please enter 'y' or 'n'." << endl;
            pf::Pause();
            cmd::RefreshBoard();
            HangmanDialogue();
            break;
        }
    }

    bool PlayHangmanGame(int hangmanX, int hangmanY, const std::string arr[] /*= randWords*/)
    {
        HangmanDialogue();

        if (!playHangman)
        {
            return true;
        }

        srand(time(NULL));
        int randNum = rand() % 150;
        std::string randWord = arr[randNum];

        return HangmanGame(hangmanX, hangmanY, randWord);
    }

    void HangmanReward()
    {
        if (rand() % 2)
        {
            pf::alien.Heal(hangmanStat);
            cout << "As a reward, Hangman heals Alien by " << hangmanStat << " health." << endl;
        }
        else
        {
            int zombieIndex = 0;

            for (int i = 0; i < pf::zombiesArray.size(); ++i)
            {
                if (!pf::zombiesArray[i].isDead)
                {
                    do
                    {
                        zombieIndex = rand() % pf::zombiesArray.size();
                    } while (pf::zombiesArray[zombieIndex].isDead);

                    break;
                }
            }

            Zombie &z = pf::zombiesArray[zombieIndex];
            z.TakeDamage(hangmanStat);

            cout << "As a reward, Hangman attacks zombie " << z.sequence << " by " << hangmanStat << " damage." << endl;
            cmd::ZombieDamagedDialogue(z, hangmanStat);
        }
    }

    bool HangmanGame(int hangmanX, int hangmanY, string secretWord)
    {
        bool win;

        // Convert to uppercase
        for (int i = 0; i < secretWord.length(); ++i)
        {
            secretWord[i] = toupper(secretWord[i]);
        }

        string word(secretWord.length(), '_');

        int lastStage = 7, stage = 0;

        cmd::RefreshBoard();
        DisplayHangman(stage);
        cout << "Word: " << word << endl;

        do
        {
            cmd::RefreshBoard();
            DisplayHangman(stage);
            cout << "Word: " << word << endl;

            char guessChar;

            cout << "Guess => ";
            cin >> guessChar;
            guessChar = toupper(guessChar);

            vector<int> positions;

            GetPositionsOfCharInWord(secretWord, guessChar, positions);
            FillWordWithCharGivenPositions(positions, guessChar, word);

            if (positions.size() == 0)
            {
                stage++;
            }

            cmd::RefreshBoard();
            DisplayHangman(stage);
            cout << "Word: " << word << endl;

        } while (word != secretWord && stage != lastStage);

        if (stage == lastStage)
        {
            win = false;

            cout << "\nYou've failed to guess the word!" << endl;
            cout << "As a punishment, Alien's turn will end." << endl;

            pf::b[hangmanY][hangmanX] = g_empty;
        }
        else
        {
            win = true;

            cout << "\nYou've guessed the word!" << endl;
            HangmanReward();
            cout << endl;
            pf::Pause();
            cmd::RefreshBoard();
        }
        cout << "Hangman left." << endl;

        playHangman = false;
        return win;
    }

}