cmake_minimum_required(VERSION 3.15)
project(ascii_baseball)

set(CMAKE_CXX_STANDARD 14)

add_executable(ascii_baseball main.cpp Game.cpp Game.h Opponent.cpp Opponent.h)