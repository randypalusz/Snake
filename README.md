# Snake
This was intended to be an exercise in creating a simple game utilizing simple 2-D ASCII console graphics as output. As long as you're on Linux/Mac and have the following dependencies on your system, you should be good to build and run!
- cmake
- ncurses
- clang/clang++
- any terminal that supports color preferably using a square font
  - using this one: http://strlen.com/square/

# Building
From the root directory, run the following:

    ./scripts/build-clang.sh
      
This will handle compilation + linking of the game executable, "snake.cc".

To run the game, execute this from the program's root:

    ./snake.cc

# Controls
 - 4 arrow keys for movement
 - ` to quit
