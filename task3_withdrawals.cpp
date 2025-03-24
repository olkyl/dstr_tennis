#include "task3_withdrawals.hpp"
#include <iostream>
#include <sstream>

using namespace std;

// Remove a player from PlayersQueue by playerID
void removePlayerFromQueue(PlayersQueue& queue, const string& playerID) {
    if (queue.isEmpty()) {
        cout << "(!) Queue is empty, no player to remove." << endl;
        return;
    }

    PlayersQueue tempQueue;
    Player* removedPlayer = nullptr;
    bool found = false;

    while (!queue.isEmpty()) {
        Player* player = queue.dequeue();
        if (player->playerID == playerID && !found) {
            removedPlayer = player;
            found = true;
        } else {
            tempQueue.enqueue(player, player->rank);
        }
    }

    // Restore remaining players to original queue
    while (!tempQueue.isEmpty()) {
        queue.enqueue(tempQueue.dequeue());
    }

    if (found) {
        cout << "(*) Player " << removedPlayer->playerID << " (" << removedPlayer->playerName << ") has been withdrawn from the tournament." << endl;
        delete removedPlayer; // Free memory
    } else {
        cout << "(!) Player ID " << playerID << " not found in the tournament." << endl;
    }
}

// Update matches in a MatchesQueue when a player withdraws
void updateMatches(MatchesQueue& matchesQueue, const string& withdrawnPlayerID) {
    if (matchesQueue.isEmpty()) return;

    MatchesQueue tempQueue;
    bool matchUpdated = false;

    while (!matchesQueue.isEmpty()) {
        Match* match = matchesQueue.dequeue();
        if (match->result == "NA") { // Only update pending matches
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

    // Restore matches to original queue
    while (!tempQueue.isEmpty()) {
        matchesQueue.enqueue(tempQueue.dequeue());
    }

    if (!matchUpdated && !matchesQueue.isEmpty()) {
        cout << "(#) No pending matches affected by withdrawal of " << withdrawnPlayerID << "." << endl;
    }
}

// Display the withdrawal menu
void displayWithdrawalMenu() {
    clearScreen();
    cout << "=== PLAYER WITHDRAWAL MENU ===" << endl;
    cout << "1. Withdraw Player" << endl;
    cout << "2. View Withdrawals" << endl;
    cout << "0. Return" << endl;
    cout << "Enter your choice: ";
}

// Handle player withdrawals
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
        choice = getChoice(2);

        switch (choice) {
            case 1: { // Withdraw Player
                cout << "Enter Player ID to withdraw: ";
                string playerID;
                cin >> playerID;
                cin.ignore();

                // Remove player from allPlayersQueue
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
                    // Add to eliminatedPlayers
                    eliminatedPlayers.enqueue(withdrawnPlayer, withdrawnPlayer->rank);

                    // Update relevant match queues
                    if (!QFmatchesQueue.isEmpty()) {
                        updateMatches(QFmatchesQueue, playerID);
                    } else if (!RRmatchesQueue.isEmpty()) {
                        updateMatches(RRmatchesQueue, playerID);
                    } else if (!KOmatchesQueue.isEmpty()) {
                        updateMatches(KOmatchesQueue, playerID);
                    } else {
                        cout << "(#) No matches scheduled yet for this tournament." << endl;
                    }

                    // Record withdrawal in stack
                    stringstream timestamp;
                    timestamp << targetYear << "-03-23"; // Placeholder, adjust as needed
                    withdrawnStack.push(playerID, timestamp.str());
                    cout << "(*) Withdrawal recorded for " << playerID << " on " << timestamp.str() << "." << endl;
                }
                break;
            }
            case 2: // View Withdrawals
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