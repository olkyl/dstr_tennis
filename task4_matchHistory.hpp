#ifndef TASK4_MATCH_HISTORY_HPP
#define TASK4_MATCH_HISTORY_HPP

#include <iostream>
#include <fstream>
#include "allStructs.hpp"
#include "task1_schedulingSystem.hpp" 
#include "task1_playerManager.hpp"

using namespace std;

// Node structure for Stack
struct Node {
    string data;
    Node* next;
};

// Stack structure for managing history
struct Stack {
    Node* top;
    Stack() { top = nullptr; }

    void push(const string& value);
    string pop();
    string peek();
    int size();
    bool isEmpty();
    void display();
};

// Global stacks for match history
extern Stack recentHistory;
extern Stack archiveHistory;

// Function declarations
void storeResultIntoHistory(MatchesQueue& KOmatchesQueue, PlayersQueue& KO_winnersQueue, int year, Player* champion);
void displayHistory();

#endif
