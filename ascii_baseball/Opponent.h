//
// Created by Christopher Cordero on 8/3/20.
//

#ifndef ASCII_BASEBALL_OPPONENT_H
#define ASCII_BASEBALL_OPPONENT_H
#include <vector>
#include <string>
#include <ctime>
#include <random>
#include <iostream>

using namespace std;

class Opponent {
public:
    Opponent();
    std::pair <int, char> hitPitch(int inning);//function for opponent to guess which pitch to hit
    std::pair <int, char> throwPitch(int inning);//function for the opponent to determine which pitch to throw
    void addHitData(std::pair <int, char> pitch);
    void addPitchData(std::pair <int, char> pitch);

private:
    std::vector<std::pair <int, char>> pitchData;//data depending on which pitches the player is not guessing

    std::vector<std::pair <int, char>> pitches;//16 combinations (4 locations, 4 pitches)
    std::vector<std::pair <int, char>> hitData;//data depending on how often a pitch is thrown
    default_random_engine randEngine;
    uniform_int_distribution<int> pitchDis;//distribution for the 16 possible pitch combos
};


#endif //ASCII_BASEBALL_OPPONENT_H
