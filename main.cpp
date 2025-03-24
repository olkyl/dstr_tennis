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
//Ara ara
// Global queues
PlayersQueue eliminatedPlayers;
PlayersQueue allPlayersQueue;
PlayersQueue QF_winnersQueue;
PlayersQueue RR_winnersQueue;
PlayersQueue KO_winnersQueue;
MatchesQueue QFmatchesQueue;
MatchesQueue RRmatchesQueue;
MatchesQueue KOmatchesQueue;
WithdrawnPlayersStack withdrawnStack; // Already correct

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

int main() {
    int targetYear = 2025;

    int previousYears[] = {2021, 2022, 2023, 2024};
    for (int year : previousYears) {
        cout << "\nSimulating tournament for year " << year << "..." << endl;
        simulatePastTournament("players_allYears.csv", year);
        freePlayerQueue(eliminatedPlayers);
    }

    loadPlayersToQueue("players_allYears.csv", allPlayersQueue, targetYear);

    int choice;
    do {
        displayMainMenu();
        choice = getChoice(4);

        switch (choice) {
            case 1:
                handleTournamentScheduling(allPlayersQueue, QF_winnersQueue, RR_winnersQueue, KO_winnersQueue,
                                           QFmatchesQueue, RRmatchesQueue, KOmatchesQueue);
                break;
            case 2:
                ticketSales();
                break;
            case 3:
                handlePlayerWithdrawals(allPlayersQueue, eliminatedPlayers, QFmatchesQueue, RRmatchesQueue,
                                        KOmatchesQueue, withdrawnStack, targetYear); // Fixed with all 7 arguments
                break;
            case 4:
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

//g++ main.cpp task1_playerManager.cpp task1_schedulingSystem.cpp task1.cpp task4_matchHistory.cpp task4_ui.cpp task3_withdrawals.cpp task2_TicketSales.cpp -o main -lcomdlg32
