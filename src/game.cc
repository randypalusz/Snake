// Copyright 2020 <randypalusz>

#include <chrono>  // NOLINT
#include <iostream>
#include <thread>  // NOLINT

#include "headers/snake.hpp"

#define SLEEP(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define WIDTH 100
#define HEIGHT 50
#define SPEED 50

int main() {
  snake::Difficulty difficulty = snake::getDifficulty(); 
  snake::Game *game = new snake::Game(WIDTH, HEIGHT, SPEED, difficulty);
  snake::Direction d = snake::right;
  int lastKey = KEY_RIGHT;
  bool terminateGame = false;
  while (true) {
    int delay = snake::maxSpeed - game->getSpeed();
    SLEEP(delay);
    switch (game->getKey(lastKey)) {
      case KEY_RIGHT:
        d = snake::right;
        lastKey = KEY_RIGHT;
        break;
      case KEY_LEFT:
        d = snake::left;
        lastKey = KEY_LEFT;
        break;
      case KEY_DOWN:
        d = snake::down;
        lastKey = KEY_DOWN;
        break;
      case KEY_UP:
        d = snake::up;
        lastKey = KEY_UP;
        break;
      case '`':
        terminateGame = true;
        break;
      default:
        d = d;
        break;
    }
    if (terminateGame || (!game->moveSnake(d))) {
      game->endGame();
      break;
    }
  }
  return 0;
}
