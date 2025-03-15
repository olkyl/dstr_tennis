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

// ✅ Function to Store Results into History.txt and Prevent Duplicates
void storeResultIntoHistory(MatchesQueue& KOmatchesQueue, PlayersQueue& KO_winnersQueue, int year, Player* champion) {
    if (KOmatchesQueue.isEmpty()) {
        cout << "(!) No match results available for year " << year << "." << endl;
        return;
    }

    // ✅ Step 1: Check if this year's data already exists (Match ID Prefix Check)
    bool yearExists = false;
    string yearPrefix = "KO" + to_string((year - 2021) * 65 + 61); // Match ID prefix

    ifstream checkFile("history.txt");
    if (checkFile.is_open()) {
        string line;
        while (getline(checkFile, line)) {
            if (line.find(yearPrefix) == 0) {  // ✅ Check if match ID prefix already exists
                yearExists = true;
                break;
            }
        }
        checkFile.close();
    }

    if (yearExists) {
        cout << "(!) Tournament data for year " << year << " already exists in history.txt. Skipping storage.\n";
        return;  // ✅ If year exists, DO NOT save duplicate results
    }

    // ✅ Step 2: Prepare new history entry
    stringstream historyEntry;
    historyEntry << "\nYear: " << year << "\n";
    historyEntry << "--------------------------------------------------\n";
    historyEntry << "No. | Match ID | Date       | Time  | Player 1  | Player 2  | Result\n";
    historyEntry << "--------------------------------------------------\n";

    // ✅ Step 3: Write KO matches in structured table format
    MatchesQueue tempQueue;
    int count = 1;
    while (!KOmatchesQueue.isEmpty()) {
        Match* match = KOmatchesQueue.dequeue();

        historyEntry << match->matchID << "  | "  // ✅ Match ID first to prevent duplication
                     << count << "   | "
                     << match->date << " | " 
                     << match->startTime << " | "
                     << match->player1 << " | "
                     << match->player2 << " | "
                     << match->result << "\n";

        tempQueue.enqueue(match);
        count++;
    }

    // Restore original queue
    while (!tempQueue.isEmpty()) {
        KOmatchesQueue.enqueue(tempQueue.dequeue());
    }

    // ✅ Step 4: Append the correct tournament champion’s name
    if (champion != nullptr) {
        historyEntry << "\nTOURNAMENT CHAMPION: " << champion->playerName 
                     << " (" << champion->playerID << ")\n";
    } else {
        historyEntry << "\nTOURNAMENT CHAMPION: Unknown\n";
    }

    // ✅ Step 5: Write updated history to `history.txt`
    ofstream historyFile("history.txt", ios::app);
    if (historyFile.is_open()) {
        historyFile << historyEntry.str();
        historyFile.close();
        cout << "(*) Tournament history for year " << year << " saved to history.txt" << endl;
    } else {
        cout << "(!) Failed to open history.txt for writing." << endl;
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
