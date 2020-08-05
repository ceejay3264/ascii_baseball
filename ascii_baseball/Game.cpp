//
// Created by Christopher Cordero on 8/2/20.
//
#include <iostream>
#include <random>
#include <ctime>
#include "Game.h"


using namespace std;

Game::Game( std::string team, int innings_){
    default_random_engine randEngine(time(0));
    uniform_int_distribution<int> nameDis(0, 6);
    uniform_int_distribution<int> homeDis(0, 1);
    randEng = randEngine;

    uniform_int_distribution<int> tempResultDis(0, 1);
    uniform_int_distribution<int> tempHalfResultDis(0, 2);
    resultDis = tempResultDis;
    halfResultDis = tempHalfResultDis;

   int num = nameDis(randEngine);
   cpuTeamName = cpuNames[num];//randomly picks CPU name

   int homeOrAway = homeDis(randEngine);
   home = homeOrAway;//sets whether or not the player will home or away team
   if(home == 1){cout << "You're the Home Team!" << endl;}
   else{cout << "You're the Away Team!" << endl;}

   cout << endl << "You'll be playing against the " << cpuTeamName << "! Good Luck!" << endl;

   playerTeamName = team;//set team name
   innings = innings_;//set innings

   for(int i=0; i<innings; i ++){//populate runs per innings vector(-1 means empty)
       cpuRuns.push_back(-1);
       playerRuns.push_back(-1);
   }

   char fielding, batting;
   if(home == 0) {batting = 'O'; fielding = 'X';}
   else {batting = 'X'; fielding = 'O';}

   this->printPlay("PLAY BALL!", fielding, batting);

}


void Game::play(){
    std::pair <int, char> tempPitch;
    char batting;
    if(home == 1) {//setting home and away score to check later if a game needs to go into extra innings
        homeScore = &playerScore;
        awayScore = &cpuScore;
        batting = 1;
    }
    else {
        awayScore = &playerScore;
        homeScore = &cpuScore;
        batting = 1;
    }

    while(this->currentInning <= innings){//will run before game goes into extra innings
        if (currentInning != 1){ printTransition(batting);}//print after the first-half inning

        if(home == 1){//if player is home team
            playerPitching();
            batting = 1;
        }
        else{//if player is away team
            playerBatting();
            batting = 0;
        }

        printTransition(batting);//between inning transition

        if((currentInning == 9) &&  (*homeScore > *awayScore)) {break;}//game is over, last half inning will not be played
        if(home == 1){//if player is home team, its now to time to bat
            playerBatting();
            batting = 0;
        }
        else{//if player is away team, its now time to pitch
            playerPitching();
            batting = 1;
        }
        currentInning++;//next inning
    }
    if(cpuScore == playerScore){//extra innings
        while(cpuScore == playerScore){
            printTransition(batting);

            if(home == 1){playerPitching(); batting = 1;}
            else {playerBatting();batting = 0;}

            printTransition(batting);

            if(home == 1){playerBatting(); batting = 0;}
            else{playerPitching(); batting = 1;}

            currentInning++;//next inning
        }
    }
    printFinalScore();
}

void Game::playerBatting(){
    std::pair <int, char> tempPitch;
    std::pair <int, char> enemyPitch;
    std::string result_;
    playerRuns[currentInning-1] = 0;//zero runs to begin the inning
    int index;
    int pScore = 0;

    while(outs<3) {
        tempPitch = guessPitch(1);//player guesses which pitch CPU will throw
        enemyPitch = enemy.throwPitch(currentInning);//CPU decides what pitch to throw
        cout << "Opponent's guess: " << enemyPitch.first << enemyPitch.second << endl;
        if(tempPitch == enemyPitch){
            index = resultDis(randEng);
            result_ = results[index];//result is a homerun or triple
        }
        else if((tempPitch.first == enemyPitch.first) || (tempPitch.second == enemyPitch.second)){//if player guesses location or pitch
            index = halfResultDis(randEng);
            result_ = halfResult[index];//possibly a single, double or out
            if(index == 2) {//if result was an out
                outs++;
                enemy.addHitData(tempPitch);//CPU remembers the pitch the player did not guess
            }
        }
        else{
            result_ = halfResult[2];//an out
            outs++;
            enemy.addHitData(tempPitch);//CPU remembers the pitch the player did not guess
        }
        pScore += updateScore(result_, '0', playerScore);//update current runs scored this inning and overall score

        printPlay(result_, 'X', '0');
    }
    //half-inning is over
    playerRuns[currentInning-1] = pScore;//runs scored this inning are recorded
    outs = 0;//outs reset
    //clear bases
    bases[0] = ' ';
    bases[1] = ' ';
    bases[2] = ' ';
}
void Game::playerPitching(){
    std::pair <int, char> tempPitch;
    std::pair <int, char> enemyPitch;
    std::string result_;
    cpuRuns[currentInning-1] = 0;//runs this inning start at zero
    int index;
    int enemyScore = 0;

    while(outs<3) {
        tempPitch = guessPitch(0);//player decides which pitch to throw
        enemyPitch = enemy.hitPitch(currentInning);//CPU guesses which pitch will be thrown
        cout << "Opponent's guess: " << enemyPitch.first << enemyPitch.second << endl;
        if(tempPitch == enemyPitch){
            index = resultDis(randEng);
            result_ = results[index];//result is homerun ortriple
        }
        else if((tempPitch.first == enemyPitch.first) || (tempPitch.second == enemyPitch.second)){//CPU guesses location or pitch
            index = halfResultDis(randEng);
            result_ = halfResult[index];//possibly a single, double or out
            if(index == 2) {outs++;}//result was an out
        }
        else{
            result_ = halfResult[2];//an out
            outs++;
        }
        enemy.addPitchData(tempPitch);//enemy remembers which pitch to throw
        enemyScore += updateScore(result_, 'X', cpuScore);//update current runs scored this inning and overall score

        printPlay(result_, '0', 'X');
    }
    cpuRuns[currentInning-1] = enemyScore; //runs scored this inning are recorded
    outs = 0;//outs are reset
    //bases are cleared
    bases[0] = ' ';
    bases[1] = ' ';
    bases[2] = ' ';

}

void Game::printPlay(std::string const result, char field, char batter){
    //Code to print the field, message and strikezone quadrants
    cout << endl << endl;

    int numSpaces = (12-result.length())/2;

    cout << "                                 ****************" << endl;
    cout << "                                 **";
    for(int i=0; i < numSpaces; i++){cout << " ";}
    cout << result;
    for(int i=0; i < numSpaces; i++){cout << " ";}
    cout << "**" << endl;
    cout << "                                 ****************" << endl << endl;

    //SINGLE
    //DOUBLE
    //TRIPLE
    //OUT
    //HOME RUN


    cout << " _ ";
    for(int i=0; i < 7; i++){cout << "-- ";}
    cout << "__ " << endl;

    cout << "|";
    for(int i=0; i < 26; i++){cout << " ";}
    cout << "\\___" << endl;


    cout << "|";
    for(int i=0; i < 30; i++){cout << " ";}
    cout << "\\_" << endl;
    cout << "|";
    for(int i=0; i < 32; i++){
        if(i==10) {cout << field; continue;}
        cout << " ";
    }
    cout << "\\__" << "                _______________" << endl;
    cout << "|";
    for(int i=0; i < 35; i++){cout << " ";}
    cout << "\\_" << "              |      |      |" << endl;
    cout << "| ";
    for(int i=0; i < 36; i++){cout << " ";}
    cout << "\\" << "             |   1  |   2  |" << endl;
    cout << "| ";
    for(int i=0; i < 37; i++){cout << " ";}
    cout << "\\" << "            |______|______|" << endl;
    cout << "| ";
    for(int i=0; i < 9; i++){cout << "_ ";}
    for(int i=0; i < 20; i++){
        if(i==11) {cout << field; continue;}
        cout << " ";
    }
    cout << "\\" << "           |      |      |" << endl;


    cout << "|/";
    for(int i=0; i < 17; i++){cout << " ";}
    cout << "\\_";
    for(int i=0; i < 20; i++){
        cout << " ";
    }
    cout << "\\" << "          |   3  |   4  |" << endl;

    cout << "|";
    for(int i=0; i < 21; i++){
        if(i==3 || i==12) {cout << field; continue;}
        cout << " ";
    }
    cout << "\\_";
    for(int i=0; i < 18; i++){cout << " ";}
    cout << "|" << "         |______|______|" << endl;

    cout << "|";
    for(int i=0; i < 23; i++){cout << " ";}
    cout << "\\";
    for(int i=0; i < 17; i++){cout << " ";}
    cout << "|" << endl;

    cout << "|";

    cout << "[" << bases[2] << "]";

    for(int i=0; i < 12; i++){cout << " ";}

    cout << "[" << bases[1] << "]";

    for(int i=0; i < 6; i++){cout << " ";}
    cout << "\\";
    for(int i=0; i < 16; i++){cout << " ";}
    cout << "|";
    cout << "         Pitches:" << endl;

    for(int i=0; i < 5; i++){
        cout << "|";
        for(int j=0; j < 24; j++) {
            if(i == 0 && j == 6){cout << "_-----"; j +=6;}
            if(i == 1 && j == 5){cout << "/       \\"; j +=9;}
            if(i == 2 && j == 4){cout << "|     \\   \\"; j +=11;}
            if(i == 3 && j == 5){cout << "\\ " << field << "      /"; j +=10;}
            if(i == 4 && j == 6){cout << "\\______/"; j +=8;}

            if((j==20 && i==1)) {cout << field; continue;}
            cout << " ";
        }
        cout << "|";
        for(int j=0; j < 16; j++) {
            if((j==7 && i==3)) {cout << field; continue;}
            cout << " ";
        }
        cout << "|";
        if(i == 0){cout << "         Fastball (F)";}
        if(i == 1){cout << "         Slider (S)";}
        if(i == 2){cout << "         Two-Seam (T)";}
        if(i == 3){cout << "         Curveball (C)";}
        cout << endl;
    }

    for(int i=0; i < 2; i++){
        cout << "|";
        for(int j=0; j < 24; j++) {
            if(i==1 && j==0){cout << "[" << batter << "]            [" << bases[0] << "]"; j+=18;}
            if((j==20 && i==0)) {cout << field; continue;}
            cout << " ";
        }

        cout << "|";
        for(int j=0; j < 15; j++) {
            cout << " ";
        }
        cout << "|" << endl;
    }



    cout << "|";
    for(int i=0; i < 23; i++){cout << "_";}
    cout << "/";
    for(int i=0; i < 16; i++){cout << "_";}
    cout << "|" << endl;


    cout << endl;
    cout << "_______________________" << endl;
    cout << "  " << playerTeamName << ": " << playerScore << endl;
    cout << "|_____________________|" << endl;
    cout << "  " << cpuTeamName << ": " << cpuScore << endl;
    cout << "|_____________________|" << endl;
    cout << "  OUTS: " << outs << " | INNING: " << currentInning << endl;
    cout << "|_________|___________|" << endl << endl;


    // "______"
    //"/        \\"
    //"|     \\   \\"
    //"\          /"
    //"\ ______/"

}

void Game::printFinalScore(){
    //code to print the final score after the last inning
    cout << "_______________FINAL SCORE_______________" << endl;
    cout << endl << cpuTeamName << endl;
    for(int i=0; i < cpuRuns.size(); i++){
        if(cpuRuns[i] == -1) cout << "[-]";
        else cout << "[" << cpuRuns[i] << "]";
    }
    cout << "| Total: " << cpuScore;

    cout << endl << endl;

    cout << "Innings: " << currentInning << endl << endl;

    cout << playerTeamName << endl;

    for(int i=0; i < playerRuns.size(); i++){
        if(playerRuns[i] == -1) cout << "[-]";
        else cout << "[" << playerRuns[i] << "]";
    }

    cout << "| Total: " << playerScore;


    cout << endl << "_________________________________________" << endl << endl;


    if(cpuScore < playerScore) {cout << "YOU WIN!!";}
    else{cout << "YOU LOSE";}
}

std::pair <int, char> Game::guessPitch(int offOrDef){
    //message depends on if player is hitting or pitching
    if(offOrDef == 1){cout << "Guess the location and pitch about to be thrown." << endl;}
    else {cout << "Enter the location and pitch you would like to throw." << endl;}

    cout << endl << "Example: 1 S (Slider in quadrant one)" << endl;
    std::pair <int, char> pitchPair;

    int loc;
    char pitch;
    while(1) {
        cin >> loc >> pitch;
        pitch = toupper(pitch);//lower case letter is converted to upper

        if ((loc > 0 && loc < 5) && (pitch == 'S' || pitch == 'C' || pitch == 'T' || pitch == 'F')) break;
        else{
            cout << "ERROR: ** Please limit your Quadrant selection to 1, 2, 3 or 4 and your pitch selection to F, S, T or C **" << endl;
            cin.clear();//clearing stream
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignoring input
        }
    }
    pitchPair = std::make_pair(loc, pitch);
    return pitchPair;
}


void Game::printTransition(char batting){
    cout << "That's 3 Outs!!" << endl << endl;

    if(batting==1){cout << "Get ready to hit!!" << endl;}
    else{cout << "You're on the mound!!" << endl;}

    cout << "__________________________________________" << endl;
    cout << endl << cpuTeamName << endl;
    for(int i=0; i < cpuRuns.size(); i++){
        if(cpuRuns[i] == -1) cout << "[-]";
        else cout << "[" << cpuRuns[i] << "]";
    }
    cout << "| Total: " << cpuScore;

    cout << endl << endl;

    cout << "Inning: " << currentInning << endl << endl;

    cout << playerTeamName << endl;

    for(int i=0; i < playerRuns.size(); i++){
        if(playerRuns[i] == -1) cout << "[-]";
        else cout << "[" << playerRuns[i] << "]";
    }

    cout << "| Total: " << playerScore;

    cout << endl << "__________________________________________" << endl << endl;
}

int Game::updateScore(std::string result, char player, int& score){
    cout << result << endl;
    int shift; //the amount of bases each player moves over
    int runs = 0;

    //checks what the message says
    if(result == "DOUBLE"){shift = 2;}
    else if(result == "SINGLE"){shift = 1;}
    else if (result == "TRIPLE"){shift = 3;}
    else if(result == "OUT!"){return 0;}
    else{
        shift = 4;
        score++;
        runs++;
    }

    int tempNum;
    for(int i = 2; i >= 0; i--){//cycles through each base starting from third base
        if(bases[i] == player) {//checks if there is a player on the base
            tempNum = (i+1) + shift;
            if(tempNum > 3) {//checks if runner crosses homeplate to score
                score++;
                runs++;
                bases[i] = ' ';
            }
            else{//runner didn't score
                bases[i] = ' ';//previous base
                bases[tempNum-1] = player;//new base the player occupies
            }
        }
    }

    if(shift < 4) bases[shift-1] = player;//base that the hitter now occupies

    return runs;
};