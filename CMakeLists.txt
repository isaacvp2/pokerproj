cmake_minimum_required(VERSION 3.16)
project(PokerProj)

set(CMAKE_CXX_STANDARD 17)


add_executable(PokerProj_Automated AutomatedPokerSimulator.cpp)
add_executable(PokerProj_Odds PokerOddsSimulator.cpp)