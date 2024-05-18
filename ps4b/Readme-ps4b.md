# PS4a: Sokoban

## Contact
Name: Nicholas Bartow
Section: 201
Time to Complete: 12 hours


## Description
This project builds upon ps4a, which created an interface for the Sokoban game. This project builds the core elements of the game
such as movement of players and boxes and win conditions.

### Features
Describe what your major decisions were and why you did things that way.
Since the window has a refresh rate, instead of changing sprites being moved around and redrawn, I kept the draw function only tied
to the Sokoban internal matrix of the gameboard. That was updated during player moves and actions and thus the draw would redraw.
I did create a struct that was used to record player positions for the undo function as well as a stack to hold the data.

### Memory
Describe how you decided to store the level data including whether you used smart pointers.
I stored the level data in a double pointer array and I did not use a smart pointer.

### Lambdas
Describe what <algorithm> functions you used and what lambda expressions you wrote.
I didn't write a viable lambda in time but the algorithm I did use often was the std::find function to see if a player or box was on a target;

### Issues
What did you have trouble with?  What did you learn?  What doesn't work?  Be honest.  You might be penalized if you claim something works and it doesn't.
I'm still having trouble with implementing lambdas in my own code. Overall the program works as intended. The player or boxes cannot go past walls, the
game checks the win conditions and it can be restarted after pressing R.

### Extra Credit
Anything special you did.  This is required to earn bonus points.
One big feature I added was the undo portion. This allowed to fix possibility where the player pushes a box into a wall and cannot move it off. The second
little feature was that the character changes positions when it moves.

## Acknowledgements
List all sources of help including the instructor or TAs, classmates, and web pages.
If you used images or other resources than the ones provided, list them here.

Brody (classmate) helped with my project.