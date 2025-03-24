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
// #include "task3_withdrawals.hpp"
using namespace std;

// Track players who lost in QF, RR, and KO
PlayersQueue eliminatedPlayers;

// Funtion to clear the terminal screen
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayMainMenu()
{
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
int getChoice(int max)
{
    int choice;
    while (!(cin >> choice) || choice < 0 || choice > max)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice. Please enter a number between 0 and " << max << ": ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

int main()
{

    // Simulate past tournaments and save results to history.txt
    int previousYears[] = {2021, 2022, 2023, 2024};
    for (int year : previousYears)
    {
        cout << "\nSimulating tournament for year " << year << "..." << endl;
        simulatePastTournament("players_allYears.csv", year);

        // Clear eliminated players between simulations
        freePlayerQueue(eliminatedPlayers);
    }

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

    do
    {
        displayMainMenu();
        choice = getChoice(4);

        switch (choice)
        {
        case 1:
        { // Task 1
            handleTournamentScheduling(
                allPlayersQueue,
                QF_winnersQueue,
                RR_winnersQueue,
                KO_winnersQueue,
                QFmatchesQueue,
                RRmatchesQueue,
                KOmatchesQueue);
            break;
        }
        case 2: // Task 2
            ticketSales();
            break;
        case 3: // Task 3
            // apa lancau handlePlayerWithdrawals(allPlayersQueue, eliminatedPlayers, withdrawnStack);
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

        if (choice != 0)
        {
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

// g++ main.cpp task1_playerManager.cpp task1_schedulingSystem.cpp task1.cpp task4_matchHistory.cpp task4_ui.cpp task2_TicketSales.cpp -o main -lcomdlg32

// g++ main.cpp task1_playerManager.cpp task1_schedulingSystem.cpp task1.cpp task3_withdrawals.cpp task4_matchHistory.cpp -o main
// got error