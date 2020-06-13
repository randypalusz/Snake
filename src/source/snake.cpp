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
  switch (d) {
    case Easy:
      speedIncrement_ = 2;
      break;
    case Medium:
      speedIncrement_ = 5;
      break;
    case Hard:
      speedIncrement_ = 7;
      break;
    case Extreme:
      speedIncrement_ = 15;
      break;
    default:
      speedIncrement_ = 5;
      break;
  }
  speedIncrement_ = 2;
  initScreen();
  initGameBoard();
}

Game::~Game() {
  endGame();
}

void Game::initScreen() {
  initscr();
  cbreak();
  timeout(0);
  // noecho();
  keypad(stdscr, TRUE);
}

void Game::endGame() {
  endwin();
}

int Game::getSpeed() {
  return speed_;
}

void Game::updateScreen() {
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      mvprintw(i, j, "%c", gameBoard_[i][j]);
    }
  }
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
    // if collision with border
  } else if ((nextPosition[0] <= 0) ||
             (nextPosition[0] >= (height_ - 1)) ||
             (nextPosition[1] <= 0) ||
             (nextPosition[1] >= (width_ - 1))) {
    return 0;
  } else if (isSnakeCollision(nextPosition)) {
    return 0;
  } else {
    gameBoard_[snake_[0][0]][snake_[0][1]] = ' ';
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
    borderLine.push_back('#');
  }
  gameBoard_.push_back(borderLine);

  // push back remaining empty space + # border
  currentLine.clear();
  currentLine.push_back('#');
  for (int j = 0; j < (width_ - 2); j++) {
    currentLine.push_back(' ');
  }
  currentLine.push_back('#');

  for (int i = 0; i < (height_ - 2); i++) {
    gameBoard_.push_back(currentLine);
  }

  // push back last line of ########
  gameBoard_.push_back(borderLine);

  addSnake();

  generateFruit();

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
  gameBoard_[fruit_[0]][fruit_[1]] = '@';
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
  if ((speed_ + speedIncrement_) < maxSpeed) {
    speed_ += speedIncrement_;
  } else {
    speed_ = maxSpeed;
  }
}

int getRandomNumber(int start, int end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(start, end);

  return distr(gen);
}

}  // namespace snake
