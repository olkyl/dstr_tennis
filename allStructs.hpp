#ifndef ALLSTRUCTS_HPP
#define ALLSTRUCTS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include "task1_playerManager.hpp"
#include <iomanip>  
using namespace std;

extern PlayersQueue eliminatedPlayers; 

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
//Task3 Han Bin

    // ----------------- Withdrawn Player Structure -----------------
struct WithdrawnPlayer {
    string playerID;
    string playerName;
    string reason; // Reason for withdrawal (e.g., injury, personal reasons)
    WithdrawnPlayer* next;
};

// ----------------- Withdrawn Players Stack -----------------
class WithdrawnPlayersStack {
private:
    WithdrawnPlayer* top;

public:
    WithdrawnPlayersStack();
    void push(const string& playerID, const string& playerName, const string& reason);
    WithdrawnPlayer* pop();
    bool isEmpty() const;
    void display() const;
};

#endif // ALLSTRUCTS_HPP