#include <iostream>
#include <ctime>
#include "Game.h"

using namespace std;

int main() {


    //instructions//
    cout << "Welcome to ASCII Baseball!"<< endl;
    cout << endl <<  "Here's how to play:" << endl;
    cout << "OFFENSE:" << endl;
    cout << "Guess a pitch to hit for each batter. If the pitch you guess is the pitch that was thrown," << endl;
    cout << "then the batter hits either a single, double, triple or homerun. If your guess is incorrect," << endl;
    cout << "then the batter will be out." << endl;
    cout << "Three outs per inning." << endl;
    cout << endl << "DEFENSE:" << endl;
    cout << "Choose a pitch to throw, but if the CPU guesses it correctly, the batter reaches base." << endl;
    cout << "If the CPU guesses incorrectly, the batter is out!" << endl;

    cout << endl <<  "Your players will look like this: 0" << endl;
    cout << endl << "The CPU's players will look like this: X" << endl;

    cout << endl << "Ready to play ball?" << endl;

    cout << endl << "Enter 'P' to continue." << endl;

    cout << endl << "Enter any other key to quit." << endl;

    //

    char next;
    cin >> next;

    if(next != 'P' && next != 'p'){return 0;}


    cout << "Enter your team name:" << endl;
    std::string teamName;
    cin >> teamName;

    cout << "How many innings would you like to play?" << endl;

    int innings;
    cin >> innings;

    Game newGame = Game(teamName, innings);//initializing new game
    newGame.play();//starting new game
    return 0;
}
