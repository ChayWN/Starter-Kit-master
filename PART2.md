# __Part 2__

## __Video Demo__

[Video Demo](https://youtu.be/1kf7NFHZ_9k).

## __Minimum Requirements__

### __Completed__

1. Game settings
2. Game Dashboard
3. Game characters
4. Game objects (Rock, Pod & Health)
5. Alien's Movement & attack
6. Game commands
7. Arrow function
8. Characters Turn
9. Zombie's Movement & attack
10. Winner function
11. Save game
12. Load game
13. Play again
14. Quit game

## __Additional Features__

1. __Witch Oject__ <br/>
    The witch with the character ```w``` is a game object that could poison the alien by 10 damage of life for 3 rounds, total accumulated damage will be 30. On the other hand, if the zombies encounter a witch, then they will receive an additional attack of 10 from the witch. In the case when zombie or the alien get poisoned, a word "poisoned" will be appearing beside them to indicate that they got poisoned, after 3 rounds, the command will show that the alien/zombie is no longer poisoned and the "poisoned" word will be removed.

2. __Hangman Object__ <br/>
    The hangman (character: ```☻```) is a special game object that introduces minigame into the existing game. As its name suggested, the hangman object offers the typical word-guessing Hangman game for player to play. Currently, it has a total of 150 possible words. When encountered, the player will be asked whether or not they want to play Hangman. Successfully guessing the word will give player one of the following rewards: 1) Heal Alien by 10 health; or 2) Attack a random zombie by 10 damage. However, failing to do so will lead to a punishment of Alien's turn being forced to end early.
    
3. __Fireball Object & Command__ <br/>
    The fireball object has the character ```o``` in the game. When found by an Alien, it increases the number of fireball the Alien owns. To use the fireball, player has to call the command ```fireball```, which asks for the row and column on the board to shoot a fireball at. Shooting a fireball will deal a 3x3 area of explosion at the desired position. The fireball explosion will not affect the Alien, but all zombies within the area will receive a damage of 5, and all game objects involved will be destroyed, leaving a set of trails which resets after the Alien's turn end. 

4. __Text Color__ <br/>
    Each objects and characters have their own unique colors to categorise them in the gameboard. For instance, health(green), rock(blue), pod(yellow), fireball(red), witch and hangman(magenta),  alien(blue) and zombie(green). The color can also be used to identify which character's turn is active, in the case when zombie 1 is active, zombie 1's character will turn green to indicate that it is active while zombie 2 color is in grey to indicate that it is not active. Additionally, the arrow next to the character can be utilised to show which character's turn is active, for instance, if alien is active then it will be in blue, otherwise the arrow will be in green if the zombie is active.

## __Contributions__

### Chay Wen Ning

1. Hangman Object
2. Fireball Object & Command

### Melody Koh Si Jie

1. Witch Oject
2. Text Color

## __Problems Encountered & Solutions__

1. __Problem :__ Difficulty in trying to determine whether or not Alien can move if it encounters Hangman. Since losing to a Hangman game will end Alien's turn and therefore prevents it from moving, we need a way to check if Alien can move.  <br/>
   __Solution :__ Return a boolean ```win``` from the Hangman's function to let the game knows if player successfully guessed the word. If yes, Alien can continue moving. Else, Alien's cannot move and its turn will end. Since the checking for Alien's ability to move is done during ```Alien::Collide()```, the Hangman's function is also called there for the returned boolean value to be retrived directly.

2. __Problem :__  Finds difficulty to color code the characters and objects in the gameboard, initially I was using a function ```SetConsoleTextAttribute(hConsole, colourCode)``` to color the text, however found out that const char variable could not store a text color. <br/>
   __Solution :__ The code utilizes the map library in C++ and ANSI escape sequences to map characters to their corresponding colors before printing it to the console, which are stored in a ```map<char><int>``` data structure with char keys and int color values. The ANSI escape sequence ```"\033[1;3"``` sets the text color, and the number following it specifies the color. After printing the colored text, The sequence ```"\033[0m"``` resets the text color back to its default value. 