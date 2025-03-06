#ifndef TASK1_HPP
#define TASK1_HPP

#include "allStructs.hpp"
#include "task1_schedulingSystem.hpp"
#include "task1_playerManager.hpp"

using namespace std;

// Display the menu for main Task 1 actions
void display_SchedulingMenu();

// Display the submenu for "Schedule Matches"
void display_SubSchedulingMenu();

// Allow admin to choose between Task 1 actions
void handleTournamentScheduling(
    PlayersQueue& allPlayersQueue, 
    PlayersQueue& QF_winnersQueue, 
    PlayersQueue& RR_winnersQueue, 
    PlayersQueue& KO_winnersQueue, 
    MatchesQueue& QFmatchesQueue, 
    MatchesQueue& RRmatchesQueue, 
    MatchesQueue& KOmatchesQueue
);

// Enable admin to schedule matches and view results
void handleMatchScheduling(
    PlayersQueue& allPlayersQueue, 
    PlayersQueue& QF_winnersQueue, 
    PlayersQueue& RR_winnersQueue, 
    PlayersQueue& KO_winnersQueue, 
    MatchesQueue& QFmatchesQueue, 
    MatchesQueue& RRmatchesQueue, 
    MatchesQueue& KOmatchesQueue
);

// Display all players who won/advanced through each round and the eliminated players
void handlePlayerProgressionView(
    PlayersQueue& allPlayersQueue, 
    PlayersQueue& QF_winnersQueue, 
    PlayersQueue& RR_winnersQueue, 
    PlayersQueue& KO_winnersQueue
);

#endif // TASK1_HPP