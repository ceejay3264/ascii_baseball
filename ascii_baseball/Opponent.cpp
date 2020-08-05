//
// Created by Christopher Cordero on 8/3/20.
//

#include "Opponent.h"

using namespace std;

Opponent::Opponent(){
    char pitchArray[4] = {'F', 'C', 'S', 'T'};//4 pitches
    std::pair <int, char> tempPair;

    default_random_engine rando(time(0));//seeding random engine
    randEngine = rando;

    uniform_int_distribution<int> tempPitchDis(0, 15);//distribution for 16 pitch combos
    pitchDis = tempPitchDis;

    for(int i=1; i<5; i++){//populating the 16 pitch combos (4 quadrants, 4 pitches)
        for(int j=0; j<4; j++) {
            tempPair = std::make_pair(i, pitchArray[j]);
            pitches.push_back(tempPair);
        }
    }
}

std::pair <int, char> Opponent::hitPitch(int inning){
    if(inning <4){//guessing a random pitch
        int pitchIndex = pitchDis(randEngine);
        return pitches[pitchIndex];
    }
    else{//after 3 innings will guess a pitch based on the most common pitches the player throws
        int b = hitData.size()-1;
        uniform_int_distribution<int> hitDis(0, b);
        int pitchIndex = hitDis(randEngine);
        for(int i=0; i<b; i++){
            cout << hitData[i].first << hitData[i].second << " ";
        }
        cout << endl;

        return hitData[pitchIndex];
    }
}

std::pair <int, char> Opponent::throwPitch(int inning){
    if(inning <4){//gonna throw a random pitch
        int pitchIndex = pitchDis(randEngine);
        return pitches[pitchIndex];
    }
    else{ //throw a pitch based on what pitch was hit the least after 3 innings
        int b = pitchData.size()-1;
        uniform_int_distribution<int> pitchDis(0, b);
        int pitchIndex = pitchDis(randEngine);
        for(int i=0; i<b; i++){
            cout << pitchData[i].first  << pitchData[i].second << " ";
        }
        cout << endl;
        return pitchData[pitchIndex];
    }
}

void Opponent::addHitData(std::pair <int, char> pitch){//setter for hitData vector
    this->hitData.push_back(pitch);
}
void Opponent::addPitchData(std::pair <int, char> pitch){//setter for pitchData vector
    this->pitchData.push_back(pitch);
}