//
// Created by Christopher Cordero on 8/2/20.
//

#ifndef ASCII_BASEBALL_GAME_H
#define ASCII_BASEBALL_GAME_H

#include <string>
#include <ctime>
#include <vector>
#include "Opponent.h"

class Game {

public:
    Game(std::string team, int innings_);
    void play();
    void printPlay(std::string const result, char field, char batter);
    void printTransition(char batting);
    void printFinalScore();
    void playerBatting();
    void playerPitching();
    std::pair <int, char> guessPitch(int offOrDef);
    int updateScore(std::string result, char player, int& score);


private:
    //Data Members
    int innings;//total number of innings in game
    std::string playerTeamName;
    std::string cpuTeamName;
    int playerScore = 0;
    int cpuScore = 0;
    int currentInning = 1;
    int outs = 0;
    int home; //1 for if the player is home. 0 if not
    char bases[3] = {' ', ' ', ' '};
    std::vector<int> playerRuns;
    std::vector<int> cpuRuns;
    std::string cpuNames[7] = {"Lions", "Sharks", "Bears", "Eagles", "Sluggers", "Rhinos", "Wolves"};
    Opponent enemy;
    std::string results[4]= {"HOME RUN!!", "TRIPLE"};
    std::string halfResult[3] = {"SINGLE", "DOUBLE", "OUT!"};
    default_random_engine randEng;
    uniform_int_distribution<int> resultDis;
    uniform_int_distribution<int> halfResultDis;
    int* homeScore;
    int* awayScore;
};


#endif //ASCII_BASEBALL_GAME_H
