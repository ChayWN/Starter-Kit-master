# __Part 1__

## __Video Demo__

[Video Demo](https://youtu.be/gK5mI_Cnfos).

## __Minimum Requirements__

### Completed

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

Additional features will be included in part 2 demonstration.

## __Contributions__

### Chay Wen Ning

1. Randomly generate game board
2. Alien movement and attack behaviour
3. Trail reset and respawn of new game objects 
4. Implementation of game object - health, arrow, and rock
5. Character's Turn
6. Save and Load Game
7. Quit Game

### Melody Koh Si Jie

1. Game Settings - default settings & change settings
2. Display Game Board
3. Display Character's Turn & Attributes
4. Zombie movement and attack behaviour
5. Implementation of game object - pod
6. Arrow Command
7. Check Winner and Play Again


## __Problems Encountered & Solutions__

1. __Problem :__ Encounters a bug on the zombie's movement that they could move even though there's an alien or another zombie beside them, how to make the zombie detect that there is an alien or border around their surroundings and change their direction. <br/>
   __Solution :__ Consulted with my teammate and searched online on the possible solution to this problem, result shows that by adding CheckForCollision() function to the zombie class, it could check for the collision with other objects.

2. __Problem :__ Finds difficulty in storing the initial attribute values of the zombies so that when the player decides to play again, the gameboard would reset back to their initial values.<br/>
   __Solution :__ Consulted with my teammate and result shows that we could add a new variable with initialLife, attack & range in the zombie class to store the initial values and include a boolean to check if the gameboard is initialising the characters attribute values for the first time .

3. __Problem :__ The game crashes when checking for collision. When the alien or zombie encounters a border, the game crashes.<br/>
   __Solution :__ After debugging, we found out that the ```obj``` variable we initialised for simplicity with the assignment of ```pf::b[y][x]``` will cause error due to the coordinates being out of the board dimensions. Hence, we restructured the code to check if the coordinate is within the board dimension and declare the variable after that.

4. __Problem :__ Difficulty in trying to access the rock-generated object. When Alien encounters a rock, a new game object is generated in the ```col::RockPickUp()``` function, which is called after many inner functions. Attempting to return the newly generated game object to the outer dialogue function for display will cause many complication.<br/>
   __Solution :__  Instead of returning the newly generated object, we assigned it to a public variable, where the dialogue function can access as needed.  