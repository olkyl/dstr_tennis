#include "allStructs.hpp"
#include "task1_schedulingSystem.hpp"
#include "task1_playerManager.hpp"
#include "task1.hpp"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;

// Track players who lost in QF, RR, and KO
PlayersQueue eliminatedPlayers;

// Funtion to clear the terminal screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMainMenu() {
    clearScreen();
    cout << "=== APU TENNIS CHAMPIONSHIP MANAGEMENT SYSTEM ===" << endl;
    cout << "1. Tournament Scheduling and Player Progression" << endl;
    cout << "2. Ticket Sales and Spectator Management" << endl;
    cout << "3. Handling Player Withdrawals" << endl;
    cout << "4. Match History Tracking" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

// Function to get a valid choice input from the user
int getChoice(int max) {
    int choice;
    while (!(cin >> choice) || choice < 0 || choice > max) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice. Please enter a number between 0 and " << max << ": ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

int main() {

    // Simulate past tournaments and save results to history.txt
    simulatePastTournament("players_allYears.csv", 2024);
    //cout << "debug" << endl;
    //cin.get();
    simulatePastTournament("players_allYears.csv", 2023);
    //cout << "debug" << endl;
    //cin.get();

    // Clear eliminated players list from previous tournaments
    freePlayerQueue(eliminatedPlayers);

    // Initialize queues for players and matches
    PlayersQueue QF_winnersQueue;
    PlayersQueue RR_winnersQueue;
    PlayersQueue KO_winnersQueue;
    MatchesQueue QFmatchesQueue;
    MatchesQueue RRmatchesQueue;
    MatchesQueue KOmatchesQueue;

    // Load 2025 participants from CSV file
    PlayersQueue allPlayersQueue;
    loadPlayersToQueue("players_allYears.csv", allPlayersQueue, 2025);

    int choice;
    
    do {
        displayMainMenu();
        choice = getChoice(4); // put 1 for now
    
        switch (choice) {
            case 1: { // Tournament Scheduling
                handleTournamentScheduling(
                    allPlayersQueue, 
                    QF_winnersQueue, 
                    RR_winnersQueue, 
                    KO_winnersQueue, 
                    QFmatchesQueue, 
                    RRmatchesQueue, 
                    KOmatchesQueue
                );
                break;
            }
            case 2: 
                break;
            case 3: 
                break;
            case 4: 
                break;
            case 0:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }

        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    } while (choice != 0);

    freePlayerQueue(allPlayersQueue);
    freePlayerQueue(QF_winnersQueue);
    freePlayerQueue(RR_winnersQueue);
    freePlayerQueue(KO_winnersQueue);
    freeMatchesQueue(QFmatchesQueue);
    freeMatchesQueue(RRmatchesQueue);
    freeMatchesQueue(KOmatchesQueue);
    freePlayerQueue(eliminatedPlayers);
    
    return 0;
}

// g++ main.cpp task1_playerManager.cpp task1_schedulingSystem.cpp task1.cpp -o main