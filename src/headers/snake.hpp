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
  Extreme,
  Invalid
};

int getRandomNumber(int, int);
Difficulty transformCharToDifficulty(char difficulty);
snake::Difficulty getDifficulty();

const char snakeChar = 'o';
const char fruitChar = '@';
const char borderChar = '#';
const char emptyChar = ' ';
const int maxSpeed = 120;
const int scorePerFruit = 10;
const int SNAKE_PAIR = 1;
const int BORDER_PAIR = 2;
const int FRUIT_PAIR = 3;
const int EMPTY_PAIR = 4;

class Game {
 public:
  Game(int width, int height, int speed, Difficulty);
  ~Game();
  void initScreen();
  void updateScreen();
  void updateScore();
  void endGame();
  int getKey(int lastKey);
  int moveSnake(Direction);
  int getSpeed();

 private:
  void initGameBoard();
  void initColors();
  void addSnake();
  void generateFruit();
  bool isSnakeCollision(const std::vector<int>&);
  void updateSpeed();
  void showSpeed();
  std::vector<std::vector<char>> gameBoard_;
  // head will be at the back of the snake_ vector
  std::vector<std::vector<int>> snake_;
  std::vector<int> fruit_;
  int width_;
  int height_;
  int speed_;
  int speedIncrement_;
  int scoreMultiplier_;
  int score_;
};

}  // namespace snake
