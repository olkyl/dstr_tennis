#include "task3_withdrawals.hpp"
#include "task1_schedulingSystem.hpp"
#include <iostream>
#include <sstream>
// SIOW HAN BIN TP067125
//github.com/BelugaFL5
using namespace std;

void updateMatches(MatchesQueue& matchesQueue, const string& withdrawnPlayerID) {
    if (matchesQueue.isEmpty()) {
        cout << "(!) No matches available in this stage to withdraw players from." << endl;
        return;
    }

    displayMatches(matchesQueue); // Step 4: Display matches
    cout << "\n";

    MatchesQueue tempQueue;
    bool matchUpdated = false;

    while (!matchesQueue.isEmpty()) {
        Match* match = matchesQueue.dequeue();
        if (match->result == "NA") {
            if (match->player1 == withdrawnPlayerID) {
                match->result = match->player2;
                match->score1 = 0;
                match->score2 = 60;
                cout << "(*) Match " << match->matchID << " updated: " << match->player2 << " wins by forfeit (60-0)." << endl;
                matchUpdated = true;
            } else if (match->player2 == withdrawnPlayerID) {
                match->result = match->player1;
                match->score1 = 60;
                match->score2 = 0;
                cout << "(*) Match " << match->matchID << " updated: " << match->player1 << " wins by forfeit (60-0)." << endl;
                matchUpdated = true;
            }
        } else if (match->player1 == withdrawnPlayerID || match->player2 == withdrawnPlayerID) {
            cout << "(!) Match " << match->matchID << " already completed, no changes made." << endl;
        }
        tempQueue.enqueue(match);
    }

    while (!tempQueue.isEmpty()) {
        matchesQueue.enqueue(tempQueue.dequeue());
    }

    if (!matchUpdated && !matchesQueue.isEmpty()) {
        cout << "(#) No pending matches affected by withdrawal of " << withdrawnPlayerID << "." << endl;
    }
}

void displayWithdrawalMenu() {
    clearScreen();
    cout << "=== PLAYER WITHDRAWAL MENU ===" << endl;
    cout << "1. Withdraw Players in Qualifying Matches" << endl;
    cout << "2. Withdraw Players in Round Robin Matches" << endl;
    cout << "3. Withdraw Players in Knockout Matches" << endl;
    cout << "4. View Withdrawals" << endl;
    cout << "0. Return" << endl;
    cout << "Enter your choice: ";
}

void handlePlayerWithdrawals(
    PlayersQueue& allPlayersQueue, 
    PlayersQueue& eliminatedPlayers, 
    MatchesQueue& QFmatchesQueue, 
    MatchesQueue& RRmatchesQueue, 
    MatchesQueue& KOmatchesQueue, 
    WithdrawnPlayersStack& withdrawnStack, 
    int targetYear
) {
    int choice;
    do {
        displayWithdrawalMenu();
        choice = getChoice(4);

        switch (choice) {
            case 1: // Qualifying
            case 2: // Round Robin
            case 3: // Knockout
            {
                MatchesQueue* targetQueue = nullptr;
                string stageName;
                if (choice == 1) {
                    targetQueue = &QFmatchesQueue;
                    stageName = "Qualifying";
                } else if (choice == 2) {
                    targetQueue = &RRmatchesQueue;
                    stageName = "Round Robin";
                } else {
                    targetQueue = &KOmatchesQueue;
                    stageName = "Knockout";
                }

                if (targetQueue->isEmpty()) {
                    cout << "(!) Must create " << stageName << " matches first in Task 1." << endl;
                } else {
                    cout << "Enter Player ID to withdraw from " << stageName << " matches: ";
                    string playerID;
                    cin >> playerID;
                    cin.ignore();

                    PlayersQueue tempQueue;
                    Player* withdrawnPlayer = nullptr;
                    bool found = false;
                    while (!allPlayersQueue.isEmpty()) {
                        Player* player = allPlayersQueue.dequeue();
                        if (player->playerID == playerID && !found) {
                            withdrawnPlayer = player;
                            found = true;
                        } else {
                            tempQueue.enqueue(player, player->rank);
                        }
                    }
                    while (!tempQueue.isEmpty()) {
                        allPlayersQueue.enqueue(tempQueue.dequeue());
                    }

                    if (!found) {
                        cout << "(!) Player ID " << playerID << " not found in the tournament." << endl;
                    } else {
                        eliminatedPlayers.enqueue(withdrawnPlayer, withdrawnPlayer->rank);
                        updateMatches(*targetQueue, playerID);
                        stringstream timestamp;
                        timestamp << targetYear; 
                        withdrawnStack.push(playerID, timestamp.str());
                        cout << "(*) Withdrawal recorded for " << playerID << " on " << timestamp.str() << "." << endl;
                    }
                }
                break;
            }
            case 4: // View Withdrawals
                withdrawnStack.display();
                break;
            case 0: // Return
                return;
        }

        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    } while (choice != 0);
}