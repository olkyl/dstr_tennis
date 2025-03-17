#ifndef ALLSTRUCTS_HPP
#define ALLSTRUCTS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>  
using namespace std;

// ----------------- Player Structure -----------------
struct Player {
    string playerID;
    string playerName;
    char gender; // "M" or "F"
    string school; // Abbreviation of school name
    int rank; // 0 if none
    Player* next;
};

// ----------------- Match Structure -----------------
struct Match {
    string matchID; // "QF", "RR", or "KO" followed by a number
    string date; // Format: "YYYY-MM-DD"
    string startTime; // Format: "HH:MM" using 24 hour system
    string player1; //ID
    string player2; // ID
    string result;  // The ID of player 1 or 2, or "NA" if not played yet
    int score1; // Score of player 1
    int score2;  // Score of player 2
    Match* next;
};
//菜就多练 git,不要逼逼赖赖o(*￣▽￣*)ブ


#endif // ALLSTRUCTS_HPP