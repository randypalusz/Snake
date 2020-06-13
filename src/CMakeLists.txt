cmake_minimum_required(VERSION 3.10) 

project(Snake)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(Sources
  source/snake.cpp
)

set(Headers
  headers/snake.hpp
)

add_library(Snake STATIC ${Sources} ${Headers})

add_executable(game.cc ${Sources} game.cc)
target_link_libraries(game.cc Snake ${CURSES_LIBRARIES})
