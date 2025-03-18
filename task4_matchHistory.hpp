#ifndef TASK4_MATCH_HISTORY_HPP
#define TASK4_MATCH_HISTORY_HPP

#include <iostream>
#include <fstream>
#include "allStructs.hpp"
#include "task1_schedulingSystem.hpp" 
#include "task1_playerManager.hpp"

using namespace std;
// ✅ Stack Definition for History Management
class Stack {
private:
    struct Node {
        string data;
        Node* next;
    };
    Node* top;
        
public:
    Stack() { top = nullptr; }
    void push(const string& value);
    string pop();
    string peek();
    bool isEmpty();
    int size();
    void display();
};
    
// ✅ Declare Global Stacks
extern Stack recentHistory;  // Stores last 5 tournaments
extern Stack archiveHistory; // Stores older tournaments
extern void clearScreen();
    
// ✅ Function to Store Results in History
void storeResultIntoHistory(MatchesQueue& KOmatchesQueue, PlayersQueue& KO_winnersQueue, int year, Player* champion);
    
// ✅ Function to Display History from Stacks
// void displayHistory();
void displayRecentHistory();
void displayArchivedHistory();
void displayYearlyMatchHistory(const string& filename, int year);
void exportMatchHistoryToPDF(const string& filename, int userYear);

#ifdef _WIN32
// ✅ NEW: Function to Open Save File Dialog (Windows Only)
string getSaveFilePath();
#endif

#endif