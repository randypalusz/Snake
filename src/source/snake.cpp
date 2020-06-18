// Copyright 2020 <randypalusz>

#include "../headers/snake.hpp"

#include <ncurses.h>

#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace snake {

Game::Game(int width, int height, int speed, Difficulty d) {
  width_ = width;
  height_ = height;
  speed_ = speed;
  score_ = 0;
  switch (d) {
    case Easy:
      speedIncrement_ = 1;
      scoreMultiplier_ = 1; 
      break;
    case Medium:
      speedIncrement_ = 2;
      scoreMultiplier_ = 2;
      break;
    case Hard:
      speedIncrement_ = 3;
      scoreMultiplier_ = 3;
      break;
    case Extreme:
      speedIncrement_ = 5;
      scoreMultiplier_ = 4;
      break;
    default:
      speedIncrement_ = 2;
      scoreMultiplier_ = 2; 
      break;
  }
  initScreen();
  initGameBoard();
}

Game::~Game() {
  endGame();
}

void Game::initScreen() {
  initscr();
  use_default_colors();
  initColors();
  curs_set(0);
  cbreak();
  timeout(0);
  keypad(stdscr, TRUE);
}

void Game::endGame() {
  endwin();
  std::cout << "Final Score: " << score_ << std::endl;
}

int Game::getSpeed() {
  return speed_;
}

void Game::updateScreen() {
  char currentChar;
  int currentPair;
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      currentChar = gameBoard_[i][j];
      switch(currentChar){
        case(snakeChar):
          currentPair = SNAKE_PAIR;
          break;
        case(borderChar):
          currentPair = BORDER_PAIR;
          break;
        case(fruitChar):
          currentPair = FRUIT_PAIR;
          break;
        case(emptyChar):
          currentPair = EMPTY_PAIR;
          break;
        default:
          currentPair = SNAKE_PAIR;;
          break;
      }
      attron(COLOR_PAIR(currentPair));
      mvprintw(i, j, "%c", gameBoard_[i][j]);
      attroff(COLOR_PAIR(currentPair));
    }
  }
  refresh();
}

void Game::updateScore() {
  // row 0, column will be the width + some number 
  std::string s = std::to_string(score_);
  char const *pchar = s.c_str();  
  mvprintw(0, width_ + 3, "%s", "Score: ");  
  mvprintw(0, width_ + 10, "%s", pchar);
  refresh();
}

void Game::showSpeed() {
  std::string max = std::to_string(maxSpeed);
  std::string s = std::to_string(speed_);
  char const *pchar = s.c_str();
  char const *maxChar = max.c_str();
  mvprintw(2, width_ + 3, "%s", "Speed/MaxSpeed: ");  
  mvprintw(2, width_ + 19, "%s/%s", pchar, maxChar);
  refresh(); 
}

int Game::getKey(int lastKey) {
  int c = getch();
  while (c == lastKey) {
    c = getch();
  }
  flushinp();
  return c;
}

int Game::moveSnake(Direction d) {
  std::vector<int> nextPosition;
  auto snakeHead = snake_.back();
  switch (d) {
    case right:
      nextPosition.push_back(snakeHead[0]);
      nextPosition.push_back(snakeHead[1] + 1);
      break;
    case down:
      nextPosition.push_back(snakeHead[0] + 1);
      nextPosition.push_back(snakeHead[1]);
      break;
    case up:
      nextPosition.push_back(snakeHead[0] - 1);
      nextPosition.push_back(snakeHead[1]);
      break;
    case left:
      nextPosition.push_back(snakeHead[0]);
      nextPosition.push_back(snakeHead[1] - 1);
      break;
    default:
      break;
  }
  // if collision with fruit
  if ((nextPosition[0] == fruit_[0]) &&
      (nextPosition[1] == fruit_[1])) {
    // extend snake
    snake_.push_back(fruit_);
    generateFruit();
    updateSpeed();
    score_ += (scorePerFruit*scoreMultiplier_); 
    updateScore();
  // if collision with border
  } else if ((nextPosition[0] <= 0) ||
             (nextPosition[0] >= (height_ - 1)) ||
             (nextPosition[1] <= 0) ||
             (nextPosition[1] >= (width_ - 1))) {
    return 0;
  } else if (isSnakeCollision(nextPosition)) {
    return 0;
  } else {
    gameBoard_[snake_[0][0]][snake_[0][1]] = emptyChar;
    snake_.erase(snake_.begin());
    snake_.push_back(nextPosition);
    gameBoard_[nextPosition[0]][nextPosition[1]] = snakeChar;
  }
  updateScreen();
  return 1;
}

void Game::initGameBoard() {
  std::vector<char> borderLine;
  std::vector<char> currentLine;
  // push back first line of ########
  for (int i = 0; i < width_; i++) {
    borderLine.push_back(borderChar);
  }
  gameBoard_.push_back(borderLine);

  // push back remaining empty space + # border
  currentLine.clear();
  currentLine.push_back(borderChar);
  for (int j = 0; j < (width_ - 2); j++) {
    currentLine.push_back(emptyChar);
  }
  currentLine.push_back(borderChar);

  for (int i = 0; i < (height_ - 2); i++) {
    gameBoard_.push_back(currentLine);
  }

  // push back last line of ########
  gameBoard_.push_back(borderLine);

  addSnake();

  generateFruit();

  updateScore();

  showSpeed();

  updateScreen();
}

void Game::addSnake() {
  // initialize snake vector here
  std::vector<int> p1{height_ / 2, 2};
  std::vector<int> p2{height_ / 2, 3};
  snake_.push_back(p1);
  snake_.push_back(p2);
  gameBoard_[snake_[0][0]][snake_[0][1]] = snakeChar;
  gameBoard_[snake_[1][0]][snake_[1][1]] = snakeChar;
}

void Game::generateFruit() {
  if (!fruit_.empty()) {
    // collision with snake case, replace with snake
    gameBoard_[fruit_[0]][fruit_[1]] = snakeChar;
    fruit_.clear();
  }
  fruit_.push_back(getRandomNumber(2, height_ - 3));
  fruit_.push_back(getRandomNumber(2, width_ - 3));
  gameBoard_[fruit_[0]][fruit_[1]] = fruitChar;
}

bool Game::isSnakeCollision(const std::vector<int>& nextPos) {
  for (auto i : snake_) {
    if (i == nextPos) {
      return true;
    }
  }
  return false;
}

void Game::updateSpeed() {
  // Clamp snake speed at 20 less than the max
  // to get minimum 20 ms delay in ticks
  if ((speed_ + speedIncrement_) < (maxSpeed - 20)) {
    speed_ += speedIncrement_;
  } else {
    speed_ = (maxSpeed - 20);
  }
  showSpeed();
}

void Game::initColors() {
  // if snake, background -1, foreground green
  // if border, background red, foreground red
  // if fruit, background -1, foreground magenta
  start_color();
  init_pair(SNAKE_PAIR, COLOR_GREEN, -1);
  init_pair(BORDER_PAIR, COLOR_RED, COLOR_RED);
  init_pair(FRUIT_PAIR, COLOR_YELLOW, -1);
  init_pair(EMPTY_PAIR, -1, -1);
}

int getRandomNumber(int start, int end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(start, end);

  return distr(gen);
}

Difficulty transformCharToDifficulty(char difficulty){
  switch(difficulty){
    case('1'): return snake::Easy;
    case('2'): return snake::Medium;
    case('3'): return snake::Hard;
    case('4'): return snake::Extreme;
    default: return snake::Invalid;
  }
}

Difficulty getDifficulty(){
  char difficultyChar = ' ';
  snake::Difficulty difficulty = snake::Invalid; 
  std::cout << "Select your difficulty... " << std::endl;
  std::cout << "  '1' = easy" << std::endl;
  std::cout << "  '2' = normal" << std::endl;
  std::cout << "  '3' = hard" << std::endl;
  std::cout << "  '4' = extreme" << std::endl;
  while(difficulty == snake::Invalid){
    std::cin >> difficultyChar;   
    difficulty = snake::transformCharToDifficulty(difficultyChar);
    if(difficulty == snake::Invalid){
      std::cout << "Invalid Difficulty Selected, try again..." << std::endl;
    }
  }
  return difficulty;
}

}  // namespace snake
