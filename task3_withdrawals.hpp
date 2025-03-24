#ifndef TASK3_WITHDRAWALS_HPP
#define TASK3_WITHDRAWALS_HPP
// SIOW HAN BIN TP067125
//github.com/BelugaFL5

#include "allStructs.hpp"
#include "task1_playerManager.hpp"
#include "task1_schedulingSystem.hpp"
#include <string>

using namespace std;

// Stack for tracking withdrawn players
class WithdrawnPlayersStack {
private:
    struct Withdrawal {
        string playerID;
        string timestamp;
        Withdrawal* next;
    };
    Withdrawal* top;

public:
    WithdrawnPlayersStack() { top = nullptr; }
    ~WithdrawnPlayersStack() {
        while (!isEmpty()) pop();
    }

    void push(const string& playerID, const string& timestamp) {
        Withdrawal* newNode = new Withdrawal;
        newNode->playerID = playerID;
        newNode->timestamp = timestamp;
        newNode->next = top;
        top = newNode;
    }

    string pop() {
        if (isEmpty()) return "";
        string playerID = top->playerID;
        Withdrawal* temp = top;
        top = top->next;
        delete temp;
        return playerID;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    void display() const {
        if (isEmpty()) {
            cout << "(#) No players have been withdrawn yet." << endl;
            return;
        }
        cout << "=== ELIMINATED PLAYERS DUE TO WITHDRAWALS ===" << endl;
        Withdrawal* current = top;
        while (current) {
            cout << "Player ID: " << current->playerID << " | Withdrawn on: " << current->timestamp << endl;
            current = current->next;
        }
    }
};

// Extend PlayersQueue with remove method
class PlayersQueue; // Forward declaration
void removePlayerFromQueue(PlayersQueue& queue, const string& playerID);

// Main function for handling withdrawals
void handlePlayerWithdrawals(
    PlayersQueue& allPlayersQueue, 
    PlayersQueue& eliminatedPlayers, 
    MatchesQueue& QFmatchesQueue, 
    MatchesQueue& RRmatchesQueue, 
    MatchesQueue& KOmatchesQueue, 
    WithdrawnPlayersStack& withdrawnStack, 
    int tournamentYear
);

extern void clearScreen();
extern int getChoice(int max);

#endif // TASK3_WITHDRAWALS_HPP