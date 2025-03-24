#include "allStructs.hpp"
#include "task1_schedulingSystem.hpp"
#include "task1_playerManager.hpp"
#include "task4_matchHistory.hpp"
#include "task1.hpp"
#include "task4_ui.hpp"
#include "task2_TicketSales.hpp"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "task3_withdrawals.hpp"
using namespace std;

// Global queues
PlayersQueue eliminatedPlayers;
PlayersQueue allPlayersQueue;
PlayersQueue QF_winnersQueue;
PlayersQueue RR_winnersQueue;
PlayersQueue KO_winnersQueue;
MatchesQueue QFmatchesQueue;
MatchesQueue RRmatchesQueue;
MatchesQueue KOmatchesQueue;

// Global stack for Task 3
WithdrawnPlayersStack withdrawnStack;

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

int getTargetYear() {
    int year;
    cout << "Enter the target year for the tournament (e.g., 2021-2025): ";
    while (!(cin >> year) || year < 2021 || year > 2025) { // Adjust range as needed
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid year. Please enter a year between 2021 and 2025: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return year;
}

int main() {
    // Get target year from user at the start
    clearScreen();
    int targetYear = getTargetYear();

    // Simulate past tournaments up to targetYear - 1
    int previousYears[] = {2021, 2022, 2023, 2024};
    for (int year : previousYears) {
        if (year < targetYear) {
            cout << "\nSimulating tournament for year " << year << "..." << endl;
            simulatePastTournament("players_allYears.csv", year);
            freePlayerQueue(eliminatedPlayers);
        }
    }

    // Load participants for the target year
    loadPlayersToQueue("players_allYears.csv", allPlayersQueue, targetYear);

    int choice;
    do {
        displayMainMenu();
        choice = getChoice(4);

        switch (choice) {
            case 1: // Task 1
                handleTournamentScheduling(allPlayersQueue, QF_winnersQueue, RR_winnersQueue, KO_winnersQueue,
                                           QFmatchesQueue, RRmatchesQueue, KOmatchesQueue);
                break;
            case 2: // Task 2
                ticketSales();
                break;
            case 3: // Task 3
                handlePlayerWithdrawals(allPlayersQueue, eliminatedPlayers, QFmatchesQueue, RRmatchesQueue,
                                        KOmatchesQueue, withdrawnStack, targetYear); // Changed to targetYear
                break;
            case 4: // Task 4
                handleMatchHistory();
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

    // Clean up
    freePlayerQueue(allPlayersQueue);
    freePlayerQueue(QF_winnersQueue);
    freePlayerQueue(RR_winnersQueue);
    freePlayerQueue(KO_winnersQueue);
    freePlayerQueue(eliminatedPlayers);
    freeMatchesQueue(QFmatchesQueue);
    freeMatchesQueue(RRmatchesQueue);
    freeMatchesQueue(KOmatchesQueue);

    return 0;
}