#include "allStructs.hpp"
#include "task1_schedulingSystem.hpp"
#include "task1_playerManager.hpp"

using namespace std;

// Function to calculate the next date from a given date
string getNextDate(const string& date) {
    // Extract year, month, and day from the input date string
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));
    
    // Determine number of days in the current month 
    int daysInMonth;
    if (month == 2) {
        daysInMonth = 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        daysInMonth = 30;
    } else {
        daysInMonth = 31;
    }
    
    // Increment the day count
    day++;
    if (day > daysInMonth) {
        // If we've exceeded the days in month, reset day to 1 and increment month
        day = 1;
        month++;
        if (month > 12) {
            // If we've exceeded month 12, reset month to 1 and increment year
            month = 1;
            year++;
        }
    }
    
    // Format the date as YYYY-MM-DD
    stringstream ss;
    ss << year << "-";
    ss << (month < 10 ? "0" : "") << month << "-";
    ss << (day < 10 ? "0" : "") << day;
    
    return ss.str();
}

// Function to increment time by minutes
string incrementTime(const string& time, int minutesToAdd) {
    // Extract hours and minutes from the input time string
    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));

    minutes += minutesToAdd; // Add the specified minutes to the current minutes

    while (minutes >= 60) { // Convert excess minutes to hours
        minutes -= 60;
        hours++;
    }

    // Ensure hours wrap around 24
    hours %= 24;

    stringstream ss; // Format the time as HH:MM and add leading zeros if needed
    ss << (hours < 10 ? "0" : "") << hours << ":";
    ss << (minutes < 10 ? "0" : "") << minutes;

    return ss.str();
}

// Function to generate a match ID based on match type (QF, RR, or KO)
string generateMatchID(const string& matchType) {
    static int globalMatchCounter = 0;  // Unique counter to track number of matches created
    globalMatchCounter++; 

    return matchType + to_string(globalMatchCounter); // Concatenate the match type with the counter to create a unique ID
}

// Function to free memory allocated for all matches in the queue
void freeMatchesQueue(MatchesQueue& matchesQueue) {
    while (!matchesQueue.isEmpty()) {
        Match* match = matchesQueue.dequeue();
        delete match; 
    }
    cout << "(*) Matches queue memory freed." << endl;
}

// Function to display all matches in a queue without permanent removal
void displayMatches(MatchesQueue& matchesQueue) {
    if (matchesQueue.isEmpty()) {
        cout << "(#) No matches in the queue." << endl;
        return;
    }
    
    cout << "----------------------------------------------------------------------------------" << endl;
    cout << "No. | Match ID | Date       | Time  | Player 1    | Player 2    | Result" << endl;
    cout << "----------------------------------------------------------------------------------" << endl;
    
    // Create a temporary queue to hold matches while displaying
    MatchesQueue original_matches;
    int count = 0;
    
    // Display all matches by dequeuing and re-enqueuing
    while (!matchesQueue.isEmpty()) {
        Match* match = matchesQueue.dequeue(); // Get next match
        count++;
        
        printf("%-4d| %-9s| %-11s| %-6s| %-12s| %-12s| %-6s\n", 
               count, 
               match->matchID.c_str(), 
               match->date.c_str(), 
               match->startTime.c_str(), 
               match->player1.c_str(), 
               match->player2.c_str(), 
               match->result.c_str());
        
        // Re-enqueue the match into temporary queue
        original_matches.enqueue(match); // Save the match to this placeholder queue
    }
    
    cout << "----------------------------------------------------------------------------------" << endl;
    
    // Restore all matches back to original queue
    while (!original_matches.isEmpty()) {
        matchesQueue.enqueue(original_matches.dequeue());
    }
}

// Function to create Qualifying Matches (QF)
// Input: playerQueue containing all players, matchQueue to store created matches
// Output: None, just populate matchQueue with new matches
void createMatches_QF(PlayersQueue& playerQueue, MatchesQueue& matchQueue) {
    if (playerQueue.isEmpty() || playerQueue.size() < 48) {
        cout << "(!) Need exactly 48 players to create Qualifying Matches." << endl;
        return;
    }
    
    // Create a temporary queue to restore all players to the playerQueue after creating matches
    PlayersQueue original_lineup;
    
    // Set the default QF matches information
    string startDate = "2025-05-25"; // First day of the QF round
    string startTime = "09:00";     // First match will start at 9 AM
    int matchDuration = 45;         // Each match will last 45 minutes
    string currentDate = startDate; // To track the date as matches are created
    string currentTime = startTime; // To track the time as matches are created

    int matchesCreated = 0;
    
    // Create matches until playerQueue has no more players
    while (playerQueue.size() >= 2) {
        // Get the first two players
        Player* player1 = playerQueue.dequeue();
        Player* player2 = playerQueue.dequeue();
        
        // Store copies of the players in the backup queue
        original_lineup.enqueue(player1);
        original_lineup.enqueue(player2);
        
        // Create a new match
        Match* newMatch = new Match();
        newMatch->matchID = generateMatchID("QF");      // Create unique match ID
        newMatch->date = currentDate;                // Assign current date
        newMatch->startTime = currentTime;           // Assign start time
        newMatch->player1 = player1->playerID;        // First player ID
        newMatch->player2 = player2->playerID;        // Second player ID
        newMatch->result = "NA";                   // Match not played yet
        newMatch->score1 = -1;                     // No scores yet
        newMatch->score2 = -1;   
        newMatch->next = nullptr;                   // End of linked list pointer
        
        // Add newly created match to matchQueue
        matchQueue.enqueue(newMatch);
        
        // Increment match counter
        matchesCreated++;
        
        // Update the startTime for next match  (add match duration)
        currentTime = incrementTime(currentTime, matchDuration);
        
        // If the time goes past 6 PM (18:00), move to next day
        if (stoi(currentTime.substr(0, 2)) >= 18) {  
            currentDate = getNextDate(currentDate);
            currentTime = startTime;
        }
    }
    
    // Handle case where we have an odd number of players
    if (!playerQueue.isEmpty()) {
        Player* remainingPlayer = playerQueue.dequeue();
        eliminatedPlayers.enqueue(remainingPlayer);  // Move the remaining player to eliminated players
        
        cout << "(!) Auto-eliminated Player " << remainingPlayer->playerName 
             << " (" << remainingPlayer->playerID << ") did not get matched due to odd number of players." << endl;
    }
    
    cout << "(*) Created " << matchesCreated << " qualifying matches." << endl;
    
    // Restore all players back to the original queue
    while (!original_lineup.isEmpty()) {
        playerQueue.enqueue(original_lineup.dequeue());
    }
}

// Function to simulate match outcomes for QF and record the winners/losers
// Input: matchQueue with existing matches to process, playersQueue with participants, winnersQueue to store winners
// Output: Updates match results in matchQueue and populates winnersQueue with winners
void getResults_QF(MatchesQueue& matchQueue, PlayersQueue& playersQueue, PlayersQueue& winnersQueue) {
    while (!winnersQueue.isEmpty()) winnersQueue.dequeue(); // Clear previous winners

    // Check if there are any matches to process
    if (matchQueue.isEmpty()) { 
        cout << "(!) No matches available to determine results." << endl;
        return;
    }

    MatchesQueue matchesPlaceholder; // Create a placeholder queue to store matches temporarily
    
    // Process each match
    while (!matchQueue.isEmpty()) {
        Match* match = matchQueue.dequeue();
        if (match->result == "NA") {
               // Generate tennis scores (15, 30, 40, 60) for each player
               match->score1 = rand() % 4 * 15 + 15;  
               match->score2 = rand() % 4 * 15 + 15;
            
            // Set the result (winner) based on the scores
            if (match->score1 > match->score2) {
                match->result = match->player1;
            } else if (match->score1 < match->score2) {
                match->result = match->player2;
            } else {
                // If the scores tie, randomly choose a winner
                match->result = (rand() % 2 == 0) ? match->player1 : match->player2;
            }
        }

        // Extract winner and loser IDs for this match
        string winnerID = match->result;
        string loserID = (winnerID == match->player1) ? match->player2 : match->player1;

        // Find the winner and loser in the players' queue
        PlayersQueue playersPlaceholder;
        Player* winner = nullptr;
        Player* loser = nullptr;

        while (!playersQueue.isEmpty()) {
            Player* player = playersQueue.dequeue();
            if (player->playerID == winnerID && !winner) {
                winner = player;
            }
            if (player->playerID == loserID && !loser) {
                loser = player;
            }
            // Copy the players into a placeholder queue
            playersPlaceholder.enqueue(player, player->rank);
        }

        // Restore players to the original queue
        while (!playersPlaceholder.isEmpty()) {
            playersQueue.enqueue(playersPlaceholder.dequeue());
        }

        // Add winner to winnersQueue
        if (winner) {
            winnersQueue.enqueue(winner, winner->rank);
        }

        // Add loser to eliminatedPlayers
        if (loser) {
            Player* eliminatedPlayer = new Player(*loser); // Deep copy
            eliminatedPlayers.enqueue(eliminatedPlayer, 1);
        }

        matchesPlaceholder.enqueue(match);
    }

    displayMatches(matchesPlaceholder); // Display all matches with results

    // Restore the matches to the original queue
    while (!matchesPlaceholder.isEmpty()) {
        matchQueue.enqueue(matchesPlaceholder.dequeue());
    }
}

// Function to create Round Robin Matches (RR)
// Input: playerQueue containing all players, matchQueue to store created matches
// Output: Populate matchQueue with new matches
void createMatches_RR(PlayersQueue& playerQueue, MatchesQueue& matchQueue) {
    if (playerQueue.isEmpty() || playerQueue.size() < 24) {
        cout << "(!) Need exactly 24 players for round robin matches." << endl;
        return;
    }
    
    // Set the default RR matches information
    string startDate = "2025-05-30";    // First day of the RR round
    string startTime = "08:00";    // First match will start at 8 AM
    const int matchDuration = 60;   // Each match will last 60 minutes
    string currentDate = startDate;     // To track the date as matches are created
    string currentTime = startTime;     // To track the time as matches are created

    // Store all players in an array temporarily
    Player* original_lineup[24];
    int playerCount = 0;

    // Create groups using circular queue 
    CircularPlayersQueue groups[6];

    // Distribute players into the 6 groups (4 players per group)
    for (int groupNum = 0; groupNum < 6; groupNum++) {
        for (int i = 0; i < 4; i++) {
            try {
                Player* player = playerQueue.dequeue();            // Get next player
                original_lineup[playerCount++] = player;            // Store player to placeholder queue
                groups[groupNum].enqueue(player);                  // Add player to current group
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
                // If failed halfway, make sure to restore any players who were already dequeued
                for (int j = 0; j < playerCount; j++) {
                    playerQueue.enqueue(original_lineup[j], original_lineup[j]->rank);
                }
                return;
            }
        }
    }

    // Create matches - ensure all players face each other within each group
    for (int groupNum = 0; groupNum < 6; groupNum++) {
        cout << "\nRound Robin Group " << (groupNum + 1) << "\nPlayers: | ";
        for (int i = 0; i < 4; i++) { // Display each player in the group
            cout << groups[groupNum].peek(i)->playerID << " | ";
        }
        cout << "\nUpcoming matches:" << endl;

        for (int i = 0; i < 3; i++) {  // First player index (0 to 2)
            for (int j = i + 1; j < 4; j++) {  // Second player index (i+1 to 3)
                Player* p1 = groups[groupNum].peek(i);
                Player* p2 = groups[groupNum].peek(j);

                // Create a new match for each pair
                Match* newMatch = new Match();
                newMatch->matchID = generateMatchID("RR");
                newMatch->date = currentDate;
                newMatch->startTime = currentTime;
                newMatch->player1 = p1->playerID;
                newMatch->player2 = p2->playerID;
                newMatch->result = "NA";
                newMatch->score1 = 0;  // Initialize scores to 0
                newMatch->score2 = 0;  // Initialize scores to 0

                matchQueue.enqueue(newMatch);

                cout << p1->playerID << " vs " << p2->playerID << endl;

                // Update the start time for the next match
                currentTime = incrementTime(currentTime, matchDuration);

                // If the time goes past 6 PM (18:00), move to next day
                if (stoi(currentTime.substr(0, 2)) >= 18) {  
                    currentDate = getNextDate(currentDate);
                    currentTime = startTime;
                }
            }
        }
    }

    // Restore all players back to the original queue
    for (int i = 0; i < playerCount; i++) {
        playerQueue.enqueue(original_lineup[i], original_lineup[i]->rank);
    }

    cout << "(*) Round Robin matches successfully created!" << endl;
}

// Function to simulate match outcomes for RR and record the winners/losers
// Input: matchQueue with existing matches to process, playersQueue with participants, winnersQueue to store winners
// Output: Updates match results in matchQueue and populates winnersQueue with winners
void getResults_RR(MatchesQueue& matchQueue, PlayersQueue& playersQueue, PlayersQueue& winnersQueue) {
    while (!winnersQueue.isEmpty()) winnersQueue.dequeue(); // Clear previous winners

    if (matchQueue.isEmpty()) {
        cout << "(!) No Round Robin matches available to determine results." << endl;
        return;
    }

    MatchesQueue matchesPlaceholder; // Create a placeholder queue to store matches temporarily
    PlayersQueue groupWinnersQueue; // Temporary queue for tracking winners
    PlayersQueue groupLosersQueue;  // Temporary queue for tracking eliminated players

    const int MAX_PLAYERS = 24;     // 24 players in RR stage (6 groups of 4)
    string playerIDs[MAX_PLAYERS];  // Array that stores player IDs
    int playerScores[MAX_PLAYERS] = {0};  // Store total scores per player
    int playerCount = 0;

    // Process all matches
    while (!matchQueue.isEmpty()) {
        Match* match = matchQueue.dequeue();

        // Generate tennis scores (15, 30, 40, 60) for each player
        match->score1 = rand() % 4 * 15 + 15;  
        match->score2 = rand() % 4 * 15 + 15;

        // Determine winner based on score
        if (match->score1 > match->score2) {
            match->result = match->player1; // Player 1 wins
        } else if (match->score1 < match->score2) {
            match->result = match->player2; // Player 2 wins
        } else {
            match->result = (rand() % 2 == 0) ? match->player1 : match->player2; // Random selection for tiebreaker
        }

        // Store ID and achieved score into the array
        bool found1 = false, found2 = false; // Flags to check if players are already in the arrays

        // IF PLAYER IS ALREADY IN THE ARRAY
        // Iterate through the player IDs array to update the total score 
        for (int i = 0; i < playerCount; i++) {
            if (playerIDs[i] == match->player1) {
                playerScores[i] += match->score1;
                found1 = true;
            }
            if (playerIDs[i] == match->player2) {
                playerScores[i] += match->score2;
                found2 = true;
            }
        }

        // IF PLAYER IS NOT IN THE ARRAY
        // Add the new player ID (and score) to the array
        if (!found1 && playerCount < MAX_PLAYERS) {
            playerIDs[playerCount] = match->player1;
            playerScores[playerCount] = match->score1;
            playerCount++;
        }
        if (!found2 && playerCount < MAX_PLAYERS) {
            playerIDs[playerCount] = match->player2;
            playerScores[playerCount] = match->score2;
            playerCount++;
        }

        matchesPlaceholder.enqueue(match); // Store the match in the placeholder queue
    }

    cout << "(*) Created " << matchesPlaceholder.size() << " Round Robin matches. " << endl;

    //  Identify the winners (based on highest total score) in each group
    for (int g = 0; g < 6; g++) {
        int maxScore = -1;
        string winnerID = "";
        
        // Check scores for all players in this group
        for (int i = g * 4; i < (g + 1) * 4; i++) {
            if (i < playerCount && playerScores[i] > maxScore) {
                maxScore = playerScores[i];     // Update highest score found
                winnerID = playerIDs[i];        // Update ID of player with highest score
            }
        }

        PlayersQueue playersPlaceholder;
        Player* winner = nullptr;

        // Iterate through each player in each group to determine winner and losers
        while (!playersQueue.isEmpty()) {
            Player* player = playersQueue.dequeue();
            
            // Check if this player is in the current group
            bool isInGroup = false;
            for (int i = g * 4; i < (g + 1) * 4; i++) {
                if (i < playerCount && player->playerID == playerIDs[i]) {
                    isInGroup = true;
                    break;
                }
            }
            
            if (isInGroup) {
                if (player->playerID == winnerID) {
                    // Winner advances to the next stage
                    winner = player;
                    winnersQueue.enqueue(player, player->rank);
                    cout << "(*) Group " << (g + 1) << " winner: " << player->playerName 
                         << " (" << player->playerID << ") " << endl;
                } else {
                    // This player is eliminated
                    Player* eliminatedPlayer = new Player(*player);
                    eliminatedPlayers.enqueue(eliminatedPlayer, 1);
                }
            }
            playersPlaceholder.enqueue(player, player->rank); // Store player in placeholder queue
        }
        
        // Restore players to original playersQueue
        while (!playersPlaceholder.isEmpty()) {
            playersQueue.enqueue(playersPlaceholder.dequeue());
        }
    }

    // Restore matches to original matchQueue
    while (!matchesPlaceholder.isEmpty()) {
        matchQueue.enqueue(matchesPlaceholder.dequeue());
    }

    displayMatches(matchQueue); // Display all matches with results
}

// Function to create Knockout Matches (KO)
// Input: playerQueue containing all players, matchQueue to store created matches
// Output: NPopulate matchQueue with new matches
void createMatches_KO(PlayersQueue& playerQueue, MatchesQueue& matchQueue) {
    if (playerQueue.isEmpty() || playerQueue.size() < 6) {
        cout << "(!) Need exactly 6 players to create Knockout Rounds." << endl;
        return;
    }

    // Create a temporary queue to restore all players to the playerQueue after creating matches
    PlayersQueue original_lineup;
    
    // Set the default KO matches information
    string startDate = "2025-06-07"; // First day of the KO round
    string startTime = "09:00";     // First match will start at 9 AM
    int matchDuration = 45;         // Each match will last 45 minutes
    string currentDate = startDate; // To track the date as matches are created
    string currentTime = startTime; // To track the time as matches are created

    int matchesCreated = 0;

    // Get the top 2 players from the queue -- directly wait at semi-finals bracket
    Player* semiFinalist1 = playerQueue.dequeue();  
    Player* semiFinalist2 = playerQueue.dequeue(); 

    // Store these players in the placeholder queue
    original_lineup.enqueue(semiFinalist1, semiFinalist1->rank);
    original_lineup.enqueue(semiFinalist2, semiFinalist2->rank);

    // Extract the other 4 players who will compete in quarterfinals
    Player* knockoutPlayers[4];

    for (int i = 0; i < 4 && !playerQueue.isEmpty(); i++) {
        knockoutPlayers[i] = playerQueue.dequeue();
        original_lineup.enqueue(knockoutPlayers[i], knockoutPlayers[i]->rank);
    }

    // Create 2 quarterfinals matches
    for (int i = 0; i < 2; i++) {
        Player* player1 = knockoutPlayers[2 * i];
        Player* player2 = knockoutPlayers[2 * i + 1];

        // Create new match 
        Match* newMatch = new Match();
        newMatch->matchID = generateMatchID("KO");     // Create unique ID 
        newMatch->date = currentDate;                // Set match date
        newMatch->startTime = currentTime;           // Both quarter-finals at same time
        newMatch->player1 = player1->playerID;       
        newMatch->player2 = player2->playerID;       
        newMatch->result = "NA";                      // Result not available yet
        newMatch->score1 = -1;                       // No scores yet
        newMatch->score2 = -1;
        newMatch->next = nullptr;                   // End of linked list pointer

        // Add match to match queue
        matchQueue.enqueue(newMatch);
        matchesCreated++;
    }

    // Create semi-final matches
    // Semi finalist 1 vs NA placeholder
    Match* semifinalMatch1 = new Match();
    semifinalMatch1->matchID = generateMatchID("KO");
    semifinalMatch1->date = currentDate;
    semifinalMatch1->startTime = incrementTime(currentTime, matchDuration);
    semifinalMatch1->player1 = semiFinalist1->playerID;
    semifinalMatch1->player2 = "NA";
    semifinalMatch1->result = "NA";  // Not played yet
    semifinalMatch1->score1 = -1;  // Leave the scores as unused for now
    semifinalMatch1->score2 = -1;
    semifinalMatch1->next = nullptr;

    matchQueue.enqueue(semifinalMatch1); // Add match to match queue
    matchesCreated++;

    // Semi finalist 2 vs NA placeholder
    Match* semifinalMatch2 = new Match();
    semifinalMatch2->matchID = generateMatchID("KO");
    semifinalMatch2->date = currentDate;
    semifinalMatch2->startTime = incrementTime(currentTime, matchDuration);
    semifinalMatch2->player1 = semiFinalist2->playerID;
    semifinalMatch2->player2 = "NA";
    semifinalMatch2->result = "NA";  // Not played yet
    semifinalMatch2->score1 = -1;  // Leave the scores as unused for now
    semifinalMatch2->score2 = -1;
    semifinalMatch2->next = nullptr;

    matchQueue.enqueue(semifinalMatch2); // Add match to match queue
    matchesCreated++;

    // Create final match placeholder
    Match* finalMatch = new Match();
    finalMatch->matchID = generateMatchID("KO");
    finalMatch->date = currentDate;
    finalMatch->startTime = currentTime = incrementTime(currentTime, matchDuration);
    finalMatch->player1 = "NA";
    finalMatch->player2 = "NA";
    finalMatch->result = "NA";  // Not played yet
    finalMatch->score1 = -1;  // Leave the scores as unused for now
    finalMatch->score2 = -1;
    finalMatch->next = nullptr;

    matchQueue.enqueue(finalMatch); // Add match to match queue
    matchesCreated++;

    cout << "(*) Created " << matchesCreated << " knockout matches." << endl;

    // Restore all players back to the original queue
    while (!original_lineup.isEmpty()) {
        playerQueue.enqueue(original_lineup.dequeue());
    }
}

// Function to simulate match outcomes for QF and record the winners/losers
// Input: matchQueue with existing matches to process, playersQueue with participants, winnersQueue to store winners
// Output: Updates match results in matchQueue and populates winnersQueue with winners
void getResults_KO(MatchesQueue& matchQueue, PlayersQueue& playersQueue, PlayersQueue& winnersQueue) {
    // Clear any previous winners from the queue
    while (!winnersQueue.isEmpty()) winnersQueue.dequeue();
    
    // Check if there are any matches to process
    if (matchQueue.isEmpty()) {
        cout << "(!) No Knockout matches available." << endl;
        return;
    }
    
    // Initialize data structures to organize the tournament brackets
    MatchesQueue matchesPlaceholder;                                // Temporary queue for matches
    Match *qf[2] = {nullptr}, *sf[2] = {nullptr};     // Arrays for quarterfinal and semifinal matches
    Match *final = nullptr;                           // Pointer for final match
    string qfWin[2] = {}, sfWin[2] = {}, champion;    // Track winners at each stage
    int qf_index = 0, sf_index = 0;                         // Counters for match arrays
    
    // Categorize matches from the queue by Quarterfinals/Semifinals/Finals stage based on player assignments
    while (!matchQueue.isEmpty()) {
        Match* match = matchQueue.dequeue();
        if (match->player1 != "NA" && match->player2 != "NA" && qf_index < 2) {
            qf[qf_index++] = match; // If both players assigned = quarterfinal
        }
        else if ((match->player1 != "NA" || match->player2 != "NA") && sf_index < 2) {
            sf[sf_index++] = match; // If one player assigned (seeded player) = semifinal
        }
        else if (match->player1 == "NA" && match->player2 == "NA") {
            final = match; // If no players assigned = final match
        }
        matchesPlaceholder.enqueue(match);  // Store match in temporary queue
    }
    
    while (!matchesPlaceholder.isEmpty()) {
        matchQueue.enqueue(matchesPlaceholder.dequeue()); // Restore all matches to original queue
    }

    // Lambda function to find a player by ID and add them to eliminated players list
    auto eliminatePlayer = [&](string id) {
        if (id.empty() || id == "NA") return;  // Skip invalid IDs
        
        PlayersQueue playersPlaceholder;     // Temporary queue
        Player* elim = nullptr;     // Pointer for eliminated player
        
        // Search for the player in the players queue
        while (!playersQueue.isEmpty()) {
            Player* player = playersQueue.dequeue();
            if (player->playerID == id && !elim) elim = new Player(*player);  // Create deep copy if found
            playersPlaceholder.enqueue(player, player->rank);  // Preserve player in temp queue
        }
        
        // Restore original players queue
        while (!playersPlaceholder.isEmpty()) playersQueue.enqueue(playersPlaceholder.dequeue());
        
        // Add eliminated player to global eliminated list
        if (elim) eliminatedPlayers.enqueue(elim, 1);
    };
    
    // Lambda function to simulate match and determine winner
    auto simulateMatch = [&](Match* match, string& winner) {
        // Skip invalid matches
        if (!match || match->player1 == "NA" || match->player2 == "NA") return;
        
        // Only simulate if match hasn't been played yet
        if (match->result == "NA") {
            // Generate tennis scores (15, 30, 45, 60)
            match->score1 = rand() % 4 * 15 + 15;  
            match->score2 = rand() % 4 * 15 + 15;
            
            // Handle tie-breaker if needed
            if (match->score1 == match->score2)
                match->result = (rand() % 2) ? match->player1 : match->player2;  // Random winner for ties
            else
                match->result = (match->score1 > match->score2) ? match->player1 : match->player2;  // Higher score wins
        }
        
        // Store winner ID and add loser to eliminated players
        winner = match->result;
        eliminatePlayer((match->result == match->player1) ? match->player2 : match->player1); 
    };
    
    // Simulate quarterfinal matches
    for (int i = 0; i < qf_index; i++) {
        simulateMatch(qf[i], qfWin[i]);
    }
    
    // Simulate semifinal matches with quarterfinal winners
    for (int i = 0; i < sf_index; i++) {
        // Update semifinal bracket with quarterfinal winner
        if (sf[i]->player2 == "NA") sf[i]->player2 = qfWin[i];
        simulateMatch(sf[i], sfWin[i]);  // Simulate match
    }
    
    // Simulate final match with semifinal winners
    if (final) {
        final->player1 = sfWin[0];  // First finalist
        final->player2 = sfWin[1];  // Second finalist
        simulateMatch(final, champion);  // Determine champion
    }
    
    // Display tournament results
    cout << "\n=== Knockout Tournament Results ===" << endl;
    displayMatches(matchQueue);
    
    // Record tournament champion in winners queue
    if (!champion.empty()) {
        PlayersQueue playersPlaceholder;
        Player* champ = nullptr;
        
        // Find champion in players queue
        while (!playersQueue.isEmpty()) {
            Player* player = playersQueue.dequeue();
            if (player->playerID == champion && !champ) {
                champ = new Player(*player);  // Create deep copy
                winnersQueue.enqueue(champ, 1);  // Add to winners queue
                
                // Display champion information
                cout << "\nTOURNAMENT CHAMPION: " << player->playerName 
                     << " (" << player->playerID << ") from " << player->school << endl;
            }
            playersPlaceholder.enqueue(player, player->rank);  // Preserve player in temp queue
        }
        
        // Restore original players queue
        while (!playersPlaceholder.isEmpty()) playersQueue.enqueue(playersPlaceholder.dequeue());
    }
    
    cout << "(*) Tournament is completed!" << endl;
}