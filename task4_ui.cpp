#include <iostream>
#include "task4_ui.hpp"
#include "task4_matchHistory.hpp" 
using namespace std;

void displayHistoryMenu() {
    clearScreen();
    cout << "=== MATCH HISTORY MENU ===" << endl;
    cout << "1. Display Recent History Matches" << endl;
    cout << "2. Display Archived History Matches" << endl;
    cout << "0. Return to Main Menu" << endl;
    cout << "Enter your choice: ";
}

void handleMatchHistory() {
    int choice;
    do {
        displayHistoryMenu();
        choice = getChoice(2);

        switch (choice) {
            case 1: // Display Recent Matches
                displayRecentHistory();
                break;
            case 2: // Display Archived Matches
                displayArchivedHistory();
                break;
            case 0: // Return to Main Menu
                return;
        }

        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    } while (choice != 0);
}
