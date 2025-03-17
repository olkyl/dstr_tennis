#include "task4_matchHistory.hpp"

// Global Stacks
Stack recentHistory;  
Stack archiveHistory;  

// ✅ Stack Functions
void Stack::push(const string& value) {
    Node* newNode = new Node();
    newNode->data = value;
    newNode->next = top;
    top = newNode;
}

string Stack::pop() {
    if (top == nullptr) return "";
    string value = top->data;
    Node* temp = top;
    top = top->next;
    delete temp;
    return value;
}

string Stack::peek() {
    return (top != nullptr) ? top->data : "";
}

int Stack::size() {
    int count = 0;
    Node* current = top;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

bool Stack::isEmpty() {
    return top == nullptr;
}

void Stack::display() {
    if (isEmpty()) {
        cout << "Stack is empty." << endl;
        return;
    }
    Node* current = top;
    while (current) {
        cout << current->data << endl;
        current = current->next;
    }
}

void storeResultIntoHistory(MatchesQueue& KOmatchesQueue, PlayersQueue& KO_winnersQueue, int year, Player* champion) {
    if (KOmatchesQueue.isEmpty()) {
        cout << "(!) No match results available for year " << year << "." << endl;
        return;
    }

    // ✅ Step 1: Check if this year already exists in `history.txt`
    bool yearExists = false;
    ifstream checkFile("history.txt");
    string yearStr = "Year: " + to_string(year);

    if (checkFile.is_open()) {
        string line;
        while (getline(checkFile, line)) {
            if (line.find(yearStr) != string::npos) {
                yearExists = true;
                break;
            }
        }
        checkFile.close();
    }

    if (yearExists) {
        cout << "(!) Tournament data for year " << year << " already exists in history.txt. Skipping storage.\n";
        return;
    }

    // ✅ Step 2: Save the new tournament entry into `history.txt`
    ofstream historyFileOut("history.txt", ios::app);
    stringstream historyEntry;
    historyEntry << "\nYear: " << year << "\n";
    historyEntry << "---------------------------------------------------------------------------------\n";
    historyEntry << "| " << setw(8) << "No. " << " | " 
                << setw(8) << "Match ID" << " | " 
                << setw(10) << "Date" << " | " 
                << setw(7) << "Time" << " | " 
                << setw(9) << "Player 1" << " | " 
                << setw(9) << "Player 2" << " | " 
                << setw(8) << "Result" << " |\n";
    historyEntry << "---------------------------------------------------------------------------------\n";

    MatchesQueue tempQueue;
    int countMatches = 1;
    while (!KOmatchesQueue.isEmpty()) {
        Match* match = KOmatchesQueue.dequeue();
        historyEntry << "| " << setw(8) << match->matchID << " | " 
                    << setw(8) << countMatches << " | "
                    << setw(10) << match->date << " | " 
                    << setw(7) << match->startTime << " | " 
                    << setw(9) << match->player1 << " | " 
                    << setw(9) << match->player2 << " | " 
                    << setw(8) << match->result << " |\n";
        tempQueue.enqueue(match);
        countMatches++;
    }
    historyEntry << "---------------------------------------------------------------------------------\n";

    if (champion != nullptr) {
        historyEntry << "\nTOURNAMENT CHAMPION: " << champion->playerName 
                     << " (" << champion->playerID << ")\n";
    } else {
        historyEntry << "\nTOURNAMENT CHAMPION: Unknown\n";
    }

    historyFileOut << historyEntry.str();
    historyFileOut.close();
    cout << "(*) Tournament history for year " << year << " saved to history.txt" << endl;

    // ✅ Step 3: Load history.txt into `tempStack` (newest tournament on top)
    Stack tempStack;
    ifstream historyFile("history.txt");
    string line, currentYearData;
    bool isYearSection = false;
    int totalYears = 0;

    cout << "\n--- DEBUG: Loading history.txt ---" << endl;

    while (getline(historyFile, line)) {
        if (line.find("Year: ") != string::npos) {
            totalYears++; // ✅ Count total years
            if (!currentYearData.empty()) {
                tempStack.push(currentYearData);  // ✅ Push previous year's data (LIFO)
                cout << "(DEBUG) Pushed to tempStack: " << currentYearData.substr(0, 15) << "...\n";
                currentYearData.clear();
            }
            isYearSection = true;
        }
        if (isYearSection) {
            currentYearData += line + "\n";  // ✅ Add lines to the current year's data
        }
    }
    if (!currentYearData.empty()) {
        tempStack.push(currentYearData);  // ✅ Push last collected year data
        cout << "(DEBUG) Pushed last year to tempStack: " << currentYearData.substr(0, 15) << "...\n";
    }
    historyFile.close();

    cout << "(DEBUG) Total years found in history.txt: " << totalYears << endl;

    // ✅ **NEW FIX: Only process archiving if total years exceed 5**
    if (totalYears > 5) {
        cout << "(DEBUG) Exceeding limit (5). Proceeding to archive oldest year...\n";
        
        Stack historyStack;
        string oldestYearData;
        while (!tempStack.isEmpty()) {
            string yearData = tempStack.pop();
            if (tempStack.isEmpty()) {  
                // ✅ This is the oldest year, move to archive
                oldestYearData = yearData;
                cout << "(DEBUG) Oldest year detected: " << oldestYearData.substr(0, 15) << "...\n";
            } else {
                historyStack.push(yearData);  // ✅ Keep latest 5 years
            }
        }

        // ✅ Step 5: Move the **oldest** year to `archive_history.txt`
        if (!oldestYearData.empty()) {
            ofstream archiveFile("archive_history.txt", ios::app);
            archiveFile << oldestYearData << "\n";
            archiveFile.close();
            cout << "(DEBUG) Archived oldest year: " << oldestYearData.substr(0, 15) << "...\n";
        }

        // ✅ Step 6: Write the correctly ordered history back to `history.txt`
        ofstream finalHistoryFileOut("history.txt", ios::trunc);
        Stack tempReverseStack; // ✅ Temporary stack to reverse order

        // 🔥 Step 6.1: Pop everything from historyStack and store in tempReverseStack
        while (!historyStack.isEmpty()) {
            tempReverseStack.push(historyStack.pop());  // ✅ This reverses the order
        }

        // 🔥 Step 6.2: Push back into historyStack in correct order (oldest first)
        while (!tempReverseStack.isEmpty()) {
            historyStack.push(tempReverseStack.pop());  // ✅ Now historyStack has the correct order
        }

        // 🔥 Step 6.3: Write everything to history.txt in correct order
        cout << "\n--- DEBUG: Final order of history.txt ---" << endl;
        while (!historyStack.isEmpty()) {
            string historyYear = historyStack.pop();  // ✅ Oldest year will be written first
            finalHistoryFileOut << historyYear << "\n";
            cout << "(DEBUG) Writing to history.txt: " << historyYear.substr(0, 15) << "...\n";
            tempReverseStack.push(historyYear);  // ✅ Keep it in memory to restore after writing
        }
        finalHistoryFileOut.close();

        // 🔥 Step 6.4: Restore historyStack so it maintains order in memory
        while (!tempReverseStack.isEmpty()) {
            historyStack.push(tempReverseStack.pop());  // ✅ Restores the stack in correct order
        }

        cout << "(*) Tournament history updated successfully!\n";

    } else {
        cout << "(DEBUG) History does not exceed 5 records. No archiving needed.\n";
    }
}

// // ✅ Function to Display Recent Match History
// void displayRecentHistory() {
//     ifstream historyFile("history.txt");

//     if (!historyFile.is_open()) {
//         cout << "(!) Error: Could not open history.txt.\n";
//         return;
//     }

//     cout << "\n=== RECENT MATCH HISTORY ===\n";
//     string line;
//     while (getline(historyFile, line)) {
//         cout << line << endl;
//     }
//     historyFile.close();
// }

// // ✅ Function to Display Archived Match History
// void displayArchivedHistory() {
//     ifstream archiveFile("archive_history.txt");

//     if (!archiveFile.is_open()) {
//         cout << "(!) No archived history found.\n";
//         return;
//     }

//     cout << "\n=== ARCHIVED MATCH HISTORY ===\n";
//     string line;
//     while (getline(archiveFile, line)) {
//         cout << line << endl;
//     }
//     archiveFile.close();
// }


// ✅ Function to Display Recent Match History with Search Option
void displayRecentHistory() {
    ifstream historyFile("history.txt");

    if (!historyFile.is_open()) {
        cout << "(!) Error: Could not open history.txt.\n";
        return;
    }

    cout << "\n=== RECENT MATCH HISTORY ===\n";
    string line;
    while (getline(historyFile, line)) {
        cout << line << endl;
    }
    historyFile.close();

    // ✅ Ask the user if they want to search for a specific year
    int year;
    cout << "\nEnter a year to search for matches (or enter 0 to return): ";
    cin >> year;
    cin.ignore();

    if (year != 0) {
        displayYearlyMatchHistory("history.txt", year);
    }
}

// ✅ Function to Display Archived Match History with Search Option
void displayArchivedHistory() {
    ifstream archiveFile("archive_history.txt");

    if (!archiveFile.is_open()) {
        cout << "(!) No archived history found.\n";
        return;
    }

    cout << "\n=== ARCHIVED MATCH HISTORY ===\n";
    string line;
    while (getline(archiveFile, line)) {
        cout << line << endl;
    }
    archiveFile.close();

    // ✅ Ask the user if they want to search for a specific year
    int year;
    cout << "\nEnter a year to search for matches (or enter 0 to return): ";
    cin >> year;
    cin.ignore();

    if (year != 0) {
        displayYearlyMatchHistory("archive_history.txt", year);
    }
}

// ✅ Function to Search for a Specific Year and Show a "New Page"
void displayYearlyMatchHistory(const string& filename, int year) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "(!) Error: Could not open " << filename << ".\n";
        return;
    }

    string yearStr = "Year: " + to_string(year);
    string line;
    bool found = false;

    // ✅ Clear Screen
    clearScreen();
    cout << "\n=== MATCH HISTORY FOR YEAR " << year << " ===\n";

    while (getline(file, line)) {
        if (line.find(yearStr) != string::npos) {
            found = true;
            cout << line << endl;

            // ✅ Print the following lines until the next "Year: " or EOF
            while (getline(file, line)) {
                if (line.find("Year: ") != string::npos) {
                    break;
                }
                cout << line << endl;
            }
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "(!) No match records found for year " << year << ".\n";
    }

    // ✅ New UI for Options after viewing the year
    cout << "\n------------------------------------------";
    cout << "\n1. Return to Match History Menu";
    cout << "\n2. Print as PDF (Coming Soon)";
    cout << "\n------------------------------------------";
    cout << "\nEnter your choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        return; // ✅ Go back to history menu
    } else if (choice == 2) {
        cout << "PDF Export Feature Coming Soon!\n"; // ✅ Placeholder for tomorrow
    }
}
