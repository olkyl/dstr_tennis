#include "task1.hpp"
#include "task1_schedulingSystem.hpp"
#include "task4_matchHistory.hpp"
#include <iostream>
#include <limits>

using namespace std;

// Declare the global queue as external
extern PlayersQueue eliminatedPlayers; 

// Defined in main.cpp
extern void clearScreen();
extern int getChoice(int max);

// Defined in task1_schedulingSystem.cpp
extern void createMatches_QF(PlayersQueue& allPlayersQueue, MatchesQueue& QFmatchesQueue, int year);
extern void createMatches_RR(PlayersQueue& QF_winnersQueue, MatchesQueue& RRmatchesQueue, int year);    
extern void createMatches_KO(PlayersQueue& RR_winnersQueue, MatchesQueue& KOmatchesQueue, int year);

void display_SchedulingMenu() {
    clearScreen();
    cout << "=== TOURNAMENT SCHEDULING AND PLAYER PROGRESSION ===" << endl;
    cout << "1. Schedule Matches" << endl;
    cout << "2. View Player Progression" << endl;
    cout << "0. Return to Main Menu" << endl;
    cout << "Enter your choice: ";
}

void display_SubSchedulingMenu() {
    clearScreen();
    cout << "========= SELECT BELOW =========" << endl;
    cout << "1. Create Qualifying Matches" << endl;
    cout << "2. View Qualifying Matches" << endl;
    cout << "3. See QF Match Results" << endl;
    cout << "================================" << endl;
    cout << "4. Create Round Robin Matches" << endl;
    cout << "5. View Round Robin Matches" << endl;
    cout << "6. See RR Match Results" << endl;
    cout << "================================" << endl;
    cout << "7. Create Knockout Matches" << endl;
    cout << "8. View Knockout Matches" << endl;
    cout << "9. See KO Match Results" << endl;
    cout << "================================" << endl;
    cout << "0. Return" << endl;
    cout << "\nEnter your choice: ";
}

// Enable admin to schedule matches and view results
void handleMatchScheduling(
    PlayersQueue& allPlayersQueue, 
    PlayersQueue& QF_winnersQueue, 
    PlayersQueue& RR_winnersQueue, 
    PlayersQueue& KO_winnersQueue, 
    MatchesQueue& QFmatchesQueue, 
    MatchesQueue& RRmatchesQueue, 
    MatchesQueue& KOmatchesQueue,
    int tournamentYear
) {
    // Static flags to track if results have been determined
    static bool QF_resultsGenerated = false;
    static bool RR_resultsGenerated = false;
    static bool KO_resultsGenerated = false;

    loadPlayersToQueue("players_allYears.csv", allPlayersQueue, tournamentYear);
    
    display_SubSchedulingMenu();
    int scheduleChoice = getChoice(9);

    switch (scheduleChoice) {
        case 1: // Create Qualifying Matches
            if (QF_resultsGenerated) {
                cout << "(!) Cannot create new Qualifying Matches after results have been generated." << endl;
            } else if (!QFmatchesQueue.isEmpty()) {
                cout << "Qualifying Matches (QF) have already been created." << endl;
            } else {
                createMatches_QF(allPlayersQueue, QFmatchesQueue, tournamentYear);
            }
            break;
        
        case 2: // View Qualifying Matches
            if (QFmatchesQueue.isEmpty()) {
                cout << "(!) No qualifying matches have been created yet." << endl;
            } else {
                displayMatches(QFmatchesQueue);
            }
            break;
        
        case 3: // See QF Match Results
            if (QFmatchesQueue.isEmpty()) {
                cout << "(!) No qualifying matches to generate results for." << endl;
            } else if (QF_resultsGenerated) {
                cout << "(!) Qualifying match results have already been generated." << endl;
                displayMatches(QFmatchesQueue);
            } else {
                while (!QF_winnersQueue.isEmpty()) {
                    QF_winnersQueue.dequeue();
                }
                getResults_QF(QFmatchesQueue, allPlayersQueue, QF_winnersQueue);
                QF_resultsGenerated = true;  // Set flag once results are generated
            }
            break;
        
        case 4: // Create Round Robin Matches
            if (!QF_resultsGenerated) {
                cout << "(!) Must generate Qualifying match results before creating Round Robin matches." << endl;
            } else if (RR_resultsGenerated) {
                cout << "(!) Cannot create new Round Robin Matches after results have been generated." << endl;
            } else if (!RRmatchesQueue.isEmpty()) {
                cout << "Round Robin Matches (RR) have already been created." << endl;
            } else if (QF_winnersQueue.isEmpty() || QF_winnersQueue.size() < 24) {
                cout << "(!) Not enough qualified players for Round Robin. Need 24 players." << endl;
            } else {
                createMatches_RR(QF_winnersQueue, RRmatchesQueue, tournamentYear);
            }
            break;
        
        case 5: // View Round Robin Matches
            if (RRmatchesQueue.isEmpty()) {
                cout << "(!) No round robin matches have been created yet." << endl;
            } else {
                displayMatches(RRmatchesQueue);
            }
            break;
        
        case 6: // See RR Match Results
            if (!QF_resultsGenerated) {
                cout << "(!) Must generate Qualifying match results first." << endl;
            } else if (RRmatchesQueue.isEmpty()) {
                cout << "(!) No round robin matches to generate results for." << endl;
            } else if (RR_resultsGenerated) {
                cout << "(!) Round Robin match results have already been generated." << endl;
                displayMatches(RRmatchesQueue);
            } else {
                while (!RR_winnersQueue.isEmpty()) {
                    RR_winnersQueue.dequeue();
                }
                getResults_RR(RRmatchesQueue, QF_winnersQueue, RR_winnersQueue);
                RR_resultsGenerated = true;  // Set flag once results are generated
            }
            break;
        
        case 7: // Create Knockout Matches
            if (!RR_resultsGenerated) {
                cout << "(!) Must generate Round Robin match results before creating Knockout matches." << endl;
            } else if (KO_resultsGenerated) {
                cout << "(!) Cannot create new Knockout Matches after results have been generated." << endl;
            } else if (!KOmatchesQueue.isEmpty()) {
                cout << "Knockout Matches (KO) have already been created." << endl;
            } else if (RR_winnersQueue.isEmpty() || RR_winnersQueue.size() < 6) {
                cout << "(!) Not enough qualified players for Knockout. Need 6 players." << endl;
            } else {
                createMatches_KO(RR_winnersQueue, KOmatchesQueue, tournamentYear);
            }
            break;
        
        case 8: // View Knockout Matches
            if (KOmatchesQueue.isEmpty()) {
                cout << "(!) No knockout matches have been created yet." << endl;
            } else {
                displayMatches(KOmatchesQueue);
            }
            break;
        
        case 9: // See KO Match Results
            if (!RR_resultsGenerated) {
                cout << "(!) Must generate Round Robin match results first." << endl;
            } else if (KOmatchesQueue.isEmpty()) {
                cout << "(!) No knockout matches to generate results for." << endl;
            } else if (KO_resultsGenerated) {
                cout << "(!) Knockout match results have already been generated." << endl;
                displayMatches(KOmatchesQueue);
            } else {
                Player* tournamentChampion = nullptr;
                PlayersQueue winnersPlaceholder;

                getResults_KO(KOmatchesQueue, RR_winnersQueue, KO_winnersQueue);
                KO_resultsGenerated = true;  // Set flag once results are generated

                // Dequeue all winners from KO_winnersQueue and store in winnersPlaceholder
                while (!KO_winnersQueue.isEmpty()) {
                    Player* winner = KO_winnersQueue.dequeue();
                    winnersPlaceholder.enqueue(winner);  // Store each winner in placeholder
                    tournamentChampion = winner;  // Last one will be the champion
                }

                // Requeue all winners back into KO_winnersQueue
                while (!winnersPlaceholder.isEmpty()) {
                    KO_winnersQueue.enqueue(winnersPlaceholder.dequeue());
                }
    
                // Store results in history
                storeResultIntoHistory(KOmatchesQueue, KO_winnersQueue, tournamentYear, tournamentChampion);
            }
            break;                  
        
        case 0: // Return
            return;
    }
}

// Display all players who won/advanced through each round and the eliminated players
void handlePlayerProgressionView(
    PlayersQueue& allPlayersQueue, 
    PlayersQueue& QF_winnersQueue, 
    PlayersQueue& RR_winnersQueue, 
    PlayersQueue& KO_winnersQueue
) {
    clearScreen();
    cout << "All Initially Registered Participants:" << endl;
    displayPlayerQueue(allPlayersQueue);
    
    cout << "\n\nQualifying Round Winners:" << endl;
    displayPlayerQueue(QF_winnersQueue);
    
    cout << "\n\nRound Robin Winners:" << endl;
    displayPlayerQueue(RR_winnersQueue);
    
    cout << "\n\nKnockout Round Winners:" << endl;
    displayPlayerQueue(KO_winnersQueue);

    cout << "\n\nEliminated Players:" << endl;
    if (eliminatedPlayers.isEmpty()) {
        cout << "\n(#) No players were eliminated from the tournament yet.\n";
    } else {
        displayPlayerQueue(eliminatedPlayers);
    }
}

// Function to get valid year input from user
int getValidYear() {
    int year;
    do {
        cout << "Enter year for the tournament (0 to cancel): ";
        cin >> year;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "(!) Invalid input. Please enter a numeric value." << endl;
            continue;
        }
        
        // Check if user wants to cancel
        if (year == 0) {
            return 0; // Stop the function
        }
        
        // Check if tournament data for this year already exists in history.txt
        bool yearExists = false;
        string yearString = to_string(year); // String to search for year
        
        ifstream checkFile("history.txt");
        if (checkFile.is_open()) {
            string line;
            while (getline(checkFile, line)) {
                // Look for lines that start with "Year: " followed by the year
                if (line.find("Year: " + yearString) != string::npos) {
                    yearExists = true;
                    break;
                }
            }
            checkFile.close();
        }
        
        if (yearExists) {
            cout << "(!) Tournament data for year " << year << " already exists." << endl;
            cout << "(!) Please choose a different year." << endl;
            continue;
        }
        
        // Check if any players have registered for this year
        bool hasPlayers = false;
        ifstream playersFile("players_allYears.csv");
        if (playersFile.is_open()) {
            string line;
            
            // Check for BOM and skip if present
            char bom[3];
            playersFile.read(bom, 3);
            if (!(bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF)) {
                playersFile.seekg(0); // If no BOM, reset file pointer to beginning
            }
            
            while (getline(playersFile, line)) {
                stringstream ss(line);
                string token;
                
                // Skip to the year field (6th column)
                for (int i = 0; i < 5; i++) {
                    getline(ss, token, ',');
                }
                
                // Parse year 
                int fileYear = 0;
                if (getline(ss, token, ',')) {
                    try {
                        fileYear = stoi(token);
                        if (fileYear == year) {
                            hasPlayers = true;
                            break;
                        }
                    } catch (...) {
                        // Skip any lines with invalid year format
                    }
                }
            }
            playersFile.close();
        }
        
        if (!hasPlayers) {
            cout << "(!) No registration has opened for year " << year << ", cannot schedule matches." << endl;
            cout << "(!) Please choose a different year." << endl;
            continue;
        }
        
        break;
    } while (true);
    return year;
}

// Allow admin to choose between Task 1 actions
void handleTournamentScheduling(
    PlayersQueue& allPlayersQueue, 
    PlayersQueue& QF_winnersQueue, 
    PlayersQueue& RR_winnersQueue, 
    PlayersQueue& KO_winnersQueue, 
    MatchesQueue& QFmatchesQueue, 
    MatchesQueue& RRmatchesQueue, 
    MatchesQueue& KOmatchesQueue
) {
    int option;
    bool yearInput = false;  // Flag to track if year has been input
    int tournamentYear = 0;  // Variable to store the tournament year
    
    do {
        display_SchedulingMenu();
        option = getChoice(2);

        switch (option) {
            case 1: // Schedule Matches
                // Ask for year input
                if (!yearInput) {
                    tournamentYear = getValidYear();
                    
                    // Check if user canceled the year input
                    if (tournamentYear == 0) {
                        cout << "(#) Match scheduling canceled." << endl;
                        break;
                    }
                    
                    yearInput = true;
                    cout << "(*) Tournament year set to: " << tournamentYear << endl;
                    cout << "Press Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                }
                
                handleMatchScheduling(
                    allPlayersQueue, 
                    QF_winnersQueue, 
                    RR_winnersQueue, 
                    KO_winnersQueue, 
                    QFmatchesQueue, 
                    RRmatchesQueue, 
                    KOmatchesQueue,
                    tournamentYear
                );
                break;
            
            case 2: // View Player Progression
                handlePlayerProgressionView(
                    allPlayersQueue, 
                    QF_winnersQueue, 
                    RR_winnersQueue, 
                    KO_winnersQueue
                );
                break;
            
            case 0: // Return to Main Menu
                return;
        }

        if (option != 0) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    } while (option != 0);
}