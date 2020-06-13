// Copyright 2020 <randypalusz>

#include <ncurses.h>

#include <string>
#include <tuple>
#include <vector>

namespace snake {

enum Direction {
  right,
  left,
  up,
  down
};

enum Difficulty {
  Easy,
  Medium,
  Hard,
  Extreme
};

int getRandomNumber(int, int);

const char snakeChar = 'o';
const int maxSpeed = 100;

class Game {
 public:
  Game(int width, int height, int speed, Difficulty);
  ~Game();
  void initScreen();
  void updateScreen();
  void endGame();
  int getKey(int lastKey);
  int moveSnake(Direction);
  int getSpeed();

 private:
  void initGameBoard();
  void addSnake();
  void generateFruit();
  bool isSnakeCollision(const std::vector<int>&);
  void updateSpeed();
  std::vector<std::vector<char>> gameBoard_;
  // head will be at the back of the snake_ vector
  std::vector<std::vector<int>> snake_;
  std::vector<int> fruit_;
  int width_;
  int height_;
  int speed_;
  int speedIncrement_;
};

}  // namespace snake
