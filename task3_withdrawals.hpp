#ifndef TASK3_WITHDRAWALS_HPP
#define TASK3_WITHDRAWALS_HPP

#include "allStructs.hpp"
#include "task1_playerManager.hpp" // Include PlayersQueue definition
#include <string>
using namespace std;

// Declare global withdrawn players stack
extern WithdrawnPlayersStack withdrawnStack;

// Function prototypes
void withdrawPlayer(PlayersQueue& allPlayersQueue, WithdrawnPlayersStack& withdrawnStack, const string& playerID, const string& reason);
void substitutePlayer(PlayersQueue& eliminatedPlayers, WithdrawnPlayersStack& withdrawnStack);
void displayWithdrawalMenu();
void handlePlayerWithdrawals(PlayersQueue& allPlayersQueue, PlayersQueue& eliminatedPlayers, WithdrawnPlayersStack& withdrawnStack);

#endif // TASK3_WITHDRAWALS_HPP