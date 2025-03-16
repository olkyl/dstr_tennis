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

// // ✅ Function to Store Results into History.txt and Prevent Duplicates
// void storeResultIntoHistory(MatchesQueue& KOmatchesQueue, PlayersQueue& KO_winnersQueue, int year, Player* champion) {
//     if (KOmatchesQueue.isEmpty()) {
//         cout << "(!) No match results available for year " << year << "." << endl;
//         return;
//     }

//     // ✅ Step 1: Check if this year already exists in `history.txt`
//     bool yearExists = false;
//     ifstream checkFile("history.txt");
//     string yearStr = to_string(year);  // Convert year to string

//     if (checkFile.is_open()) {
//         string line;
//         while (getline(checkFile, line)) {
//             if (line.find(yearStr) != string::npos) {  // ✅ Check if the year is already in history
//                 yearExists = true;
//                 break;
//             }
//         }
//         checkFile.close();
//     }

//     if (yearExists) {
//         cout << "(!) Tournament data for year " << year << " already exists in history.txt. Skipping storage.\n";
//         return;  // ✅ Prevents duplicate storage
//     }

//     // ✅ Step 2: Prepare new history entry
//     stringstream historyEntry;
//     historyEntry << "\nYear: " << year << "\n";
//     historyEntry << "---------------------------------------------------------------------------------\n";
//     historyEntry << "| " << setw(8) << "No. " << " | " 
//                 << setw(8) << "Match ID" << " | " 
//                 << setw(10) << "Date" << " | " 
//                 << setw(7) << "Time" << " | " 
//                 << setw(9) << "Player 1" << " | " 
//                 << setw(9) << "Player 2" << " | " 
//                 << setw(8) << "Result" << " |\n";
//     historyEntry << "---------------------------------------------------------------------------------\n";

//     // ✅ Step 3: Write KO matches in structured format
//     MatchesQueue tempQueue;
//     int count = 1;
//     while (!KOmatchesQueue.isEmpty()) {
//         Match* match = KOmatchesQueue.dequeue();

//         historyEntry << "| " << setw(8) << match->matchID << " | " 
//                     << setw(8) << count << " | "
//                     << setw(10) << match->date << " | " 
//                     << setw(7) << match->startTime << " | " 
//                     << setw(9) << match->player1 << " | " 
//                     << setw(9) << match->player2 << " | " 
//                     << setw(8) << match->result << " |\n";

//         tempQueue.enqueue(match);
//         count++;
//     }
//     historyEntry << "---------------------------------------------------------------------------------\n";

//     // Restore original queue
//     while (!tempQueue.isEmpty()) {
//         KOmatchesQueue.enqueue(tempQueue.dequeue());
//     }

//     // ✅ Step 4: Append the correct tournament champion’s name
//     if (champion != nullptr) {
//         historyEntry << "\nTOURNAMENT CHAMPION: " << champion->playerName 
//                      << " (" << champion->playerID << ")\n";
//     } else {
//         historyEntry << "\nTOURNAMENT CHAMPION: Unknown\n";
//     }

//     // ✅ Step 5: Write updated history to `history.txt`
//     ofstream historyFile("history.txt", ios::app);
//     if (historyFile.is_open()) {
//         historyFile << historyEntry.str();
//         historyFile.close();
//         cout << "(*) Tournament history for year " << year << " saved to history.txt" << endl;
//     } else {
//         cout << "(!) Failed to open history.txt for writing." << endl;
//     }
// }

// (beta working 50%)
// void storeResultIntoHistory(MatchesQueue& KOmatchesQueue, PlayersQueue& KO_winnersQueue, int year, Player* champion) {
//     if (KOmatchesQueue.isEmpty()) {
//         cout << "(!) No match results available for year " << year << "." << endl;
//         return;
//     }

//     // ✅ Step 1: Check if this year already exists in `history.txt`
//     bool yearExists = false;
//     ifstream checkFile("history.txt");
//     string yearStr = "Year: " + to_string(year);

//     if (checkFile.is_open()) {
//         string line;
//         while (getline(checkFile, line)) {
//             if (line.find(yearStr) != string::npos) {
//                 yearExists = true;
//                 break;
//             }
//         }
//         checkFile.close();
//     }

//     if (yearExists) {
//         cout << "(!) Tournament data for year " << year << " already exists in history.txt. Skipping storage.\n";
//         return;
//     }

//     // ✅ Step 2: Save the new tournament entry directly into history.txt **before** handling stacks
//     ofstream historyFileOut("history.txt", ios::app);
//     stringstream historyEntry;
//     historyEntry << "\nYear: " << year << "\n";
//     historyEntry << "---------------------------------------------------------------------------------\n";
//     historyEntry << "| " << setw(8) << "No. " << " | " 
//                 << setw(8) << "Match ID" << " | " 
//                 << setw(10) << "Date" << " | " 
//                 << setw(7) << "Time" << " | " 
//                 << setw(9) << "Player 1" << " | " 
//                 << setw(9) << "Player 2" << " | " 
//                 << setw(8) << "Result" << " |\n";
//     historyEntry << "---------------------------------------------------------------------------------\n";

//     MatchesQueue tempQueue;
//     int countMatches = 1;
//     while (!KOmatchesQueue.isEmpty()) {
//         Match* match = KOmatchesQueue.dequeue();
//         historyEntry << "| " << setw(8) << match->matchID << " | " 
//                     << setw(8) << countMatches << " | "
//                     << setw(10) << match->date << " | " 
//                     << setw(7) << match->startTime << " | " 
//                     << setw(9) << match->player1 << " | " 
//                     << setw(9) << match->player2 << " | " 
//                     << setw(8) << match->result << " |\n";
//         tempQueue.enqueue(match);
//         countMatches++;
//     }
//     historyEntry << "---------------------------------------------------------------------------------\n";

//     if (champion != nullptr) {
//         historyEntry << "\nTOURNAMENT CHAMPION: " << champion->playerName 
//                      << " (" << champion->playerID << ")\n";
//     } else {
//         historyEntry << "\nTOURNAMENT CHAMPION: Unknown\n";
//     }

//     historyFileOut << historyEntry.str();
//     historyFileOut.close();
//     cout << "(*) Tournament history for year " << year << " saved to history.txt" << endl;

//     // ✅ Step 3: Load history.txt into `tempStack` (newest tournament on top)
//     Stack tempStack;
//     ifstream historyFile("history.txt");
//     string line, currentYearData;
//     bool isYearSection = false;
//     int totalYears = 0;

//     cout << "\n--- DEBUG: Loading history.txt ---" << endl;

//     while (getline(historyFile, line)) {
//         if (line.find("Year: ") != string::npos) {
//             totalYears++; // ✅ Count total years
//             if (!currentYearData.empty()) {
//                 tempStack.push(currentYearData);  // ✅ Push previous year's data (LIFO)
//                 cout << "(DEBUG) Pushed to tempStack: " << currentYearData.substr(0, 15) << "...\n";
//                 currentYearData.clear();
//             }
//             isYearSection = true;
//         }
//         if (isYearSection) {
//             currentYearData += line + "\n";  // ✅ Add lines to the current year's data
//         }
//     }
//     if (!currentYearData.empty()) {
//         tempStack.push(currentYearData);  // ✅ Push last collected year data
//         cout << "(DEBUG) Pushed last year to tempStack: " << currentYearData.substr(0, 15) << "...\n";
//     }
//     historyFile.close();

//     cout << "(DEBUG) Total years found in history.txt: " << totalYears << endl;

//     // ✅ Step 4: Archive oldest year if more than 5 exist
//     Stack historyStack;
//     string oldestYearData;
//     while (!tempStack.isEmpty()) {
//         string yearData = tempStack.pop();
//         if (tempStack.isEmpty()) {  
//             // ✅ This is the oldest year, move to archive
//             oldestYearData = yearData;
//             cout << "(DEBUG) Oldest year detected: " << oldestYearData.substr(0, 15) << "...\n";
//         } else {
//             historyStack.push(yearData);  // ✅ Keep latest 5 years
//         }
//     }

//     // ✅ Step 5: Move the **oldest** year to `archive_history.txt`
//     if (!oldestYearData.empty()) {
//         ofstream archiveFile("archive_history.txt", ios::app);
//         archiveFile << oldestYearData << "\n";
//         archiveFile.close();
//         cout << "(DEBUG) Archived oldest year: " << oldestYearData.substr(0, 15) << "...\n";
//     }

//     // ✅ Step 6: Write the correctly ordered history back to `history.txt`
//     ofstream finalHistoryFileOut("history.txt", ios::trunc);
//     Stack finalHistoryStack;
//     while (!historyStack.isEmpty()) {
//         finalHistoryStack.push(historyStack.pop());  // ✅ Reverse order
//     }

//     cout << "\n--- DEBUG: Final order of history.txt ---" << endl;
//     while (!finalHistoryStack.isEmpty()) {
//         string historyYear = finalHistoryStack.pop();
//         finalHistoryFileOut << historyYear << "\n";  // ✅ Write in correct order (oldest first)
//         cout << "(DEBUG) Writing to history.txt: " << historyYear.substr(0, 15) << "...\n";
//     }
//     finalHistoryFileOut.close();

//     cout << "(*) Tournament history updated successfully!\n";
// }

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


// ✅ Function to Display History (Both Stacks)
void displayHistory() {
    ifstream historyFile("history.txt");

    if (!historyFile.is_open()) {
        cout << "(!) Error: Could not open history.txt.\n";
        return;
    }

    cout << "\n=== Recent Tournament Match History ===\n";
    string line;
    while (getline(historyFile, line)) {
        cout << line << endl;
    }
    historyFile.close();

    // ✅ Display Archived History (If Exists)
    ifstream archiveFile("archive_history.txt");
    if (archiveFile.is_open()) {
        cout << "\n=== Archived Tournament Match History ===\n";
        while (getline(archiveFile, line)) {
            cout << line << endl;
        }
        archiveFile.close();
    }
}
