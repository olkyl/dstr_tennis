#ifdef _WIN32  // Only for Windows
#include <windows.h>
#include <commdlg.h>  // Required for file dialog
#endif

#include "task4_matchHistory.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <thread> 
#include <chrono> 
using namespace std;
namespace fs = std::filesystem;
string originalDirectory = fs::current_path().string();
// Global Stacks
Stack recentHistory;  
Stack archiveHistory;  
string lastSavedFilePath = ""; 

extern string lastSavedFilePath;

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

void displayRecentHistory() {
    cout << "(*) DEBUG: Checking if history.txt exists...\n";

    if (!fs::exists("history.txt")) {
        cerr << "(!) ERROR: history.txt DOES NOT EXIST in: " << fs::absolute("history.txt") << endl;
        cerr << "Ensure the file is properly stored.\n";
        return;
    }

    ifstream historyFile("history.txt");

    if (!historyFile.is_open()) {
        cerr << "(!) ERROR: Cannot open history.txt. File may not exist or is empty.\n";
        return;
    }

    cout << "\n=== RECENT MATCH HISTORY ===\n";
    string line;
    bool hasData = false;

    while (getline(historyFile, line)) {
        if (!line.empty()) {
            hasData = true;
            cout << line << endl;
        }
    }
    historyFile.close();  // ✅ Ensure the file is properly closed

    if (!hasData) {
        cerr << "(!) ERROR: history.txt exists but is empty.\n";
        return;
    }

    // ✅ Ask the user if they want to search for a specific year
    int year;
    cout << "\nEnter a year to search for matches (or enter 0 to return): ";
    cin >> year;
    cin.ignore();

    if (year != 0) {
        displayYearlyMatchHistory("history.txt", year);
    }
}

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
    cout << "\nEnter a year to search for matches in archive (or enter 0 to return): ";
    cin >> year;
    cin.ignore();

    if (year != 0) {
        // 🔥 Ensure we pass the correct filename
        displayYearlyMatchHistory("archive_history.txt", year);  
    }
}


void displayYearlyMatchHistory(const string& filename, int year) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "(!) Error: Could not open " << filename << ".\n";
        return;
    }

    string yearStr = "Year: " + to_string(year);
    string line;
    bool found = false;
    stringstream yearData;  // Store the history for this year

    // ✅ Clear Screen
    clearScreen();
    cout << "\n=== MATCH HISTORY FOR YEAR " << year << " ===\n";

    while (getline(file, line)) {
        if (line.find(yearStr) != string::npos) {
            found = true;
            cout << line << endl;
            yearData << line << "\n";  // Store for PDF export

            // ✅ Print the following lines until the next "Year: " or EOF
            while (getline(file, line)) {
                if (line.find("Year: ") != string::npos) {
                    break;
                }
                cout << line << endl;
                yearData << line << "\n";  // Store for PDF export
            }
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "(!) No match records found for year " << year << ".\n";
        return;
    }

    // ✅ New UI for Options after viewing the year
    cout << "\n------------------------------------------";
    cout << "\n1. Return to Match History Menu";
    cout << "\n2. Print as PDF";
    cout << "\n------------------------------------------";
    cout << "\nEnter your choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        return; // ✅ Go back to history menu
    } else if (choice == 2) {
        cout << "Exporting match history to PDF for year " << year << "...\n";
        
        // ✅ Pass the correct filename (history.txt OR archive_history.txt)
        exportMatchHistoryToPDF(filename, year);  
    }
}

// void exportMatchHistoryToPDF(const string& filename, int userYear) {
//     cout << "Current Working Directory: " << fs::current_path() << endl;

//     if (!fs::exists(filename)) {
//         cerr << "(!) ERROR: File does not exist: " << fs::absolute(filename) << endl;
//         return;
//     }

//     ifstream historyFile(filename);
//     if (!historyFile.is_open()) {
//         cerr << "ERROR: Cannot open file: " << filename << endl;
//         return;
//     }

//     #ifdef _WIN32
//         string savePath = getSaveFilePath();
//         if (savePath.empty()) {
//             cout << "Cancelled PDF save.\n";
//             historyFile.close();
//             return;
//         }
//     #else
//         string savePath = "match_history.pdf";
//     #endif

//     ofstream pdfFile(savePath, ios::binary);
//     if (!pdfFile.is_open()) {
//         cerr << "ERROR: Cannot create PDF file: " << savePath << endl;
//         historyFile.close();
//         return;
//     }

//     // ✅ PDF Formatting
//     stringstream textStream;
//     textStream << "BT /F1 12 Tf 50 800 Td (Match History Report for " << userYear << ") Tj ET\n";
//     textStream << "BT /F1 10 Tf 50 780 Td (Year: " << userYear << ") Tj ET\n";
//     textStream << "BT /F1 10 Tf 50 760 Td (------------------------------------------------------------) Tj ET\n";
//     textStream << "BT /F1 10 Tf 50 740 Td (| No. | Match ID | Date | Time | Player 1 | Player 2 | Result |) Tj ET\n";
//     textStream << "BT /F1 10 Tf 50 720 Td (------------------------------------------------------------) Tj ET\n";

//     string yearToFind = "Year: " + to_string(userYear);
//     bool foundYear = false;
//     string line;
//     int yPosition = 700;

//     while (getline(historyFile, line)) {
//         if (line.find("Year: ") != string::npos) {
//             if (line.find(yearToFind) != string::npos) {
//                 foundYear = true;
//                 continue;
//             } else if (foundYear) {
//                 break;
//             }
//         }
//         if (foundYear) {
//             textStream << "BT /F1 10 Tf 50 " << yPosition << " Td (" << line << ") Tj ET\n";
//             yPosition -= 20;
//         }
//     }

//     historyFile.close();

//     if (!foundYear) {
//         cerr << "(!) ERROR: Year " << userYear << " not found in " << filename << ".\n";
//         return;
//     }

//     textStream << "BT /F1 10 Tf 50 " << yPosition << " Td (------------------------------------------------------------) Tj ET\n";

//     pdfFile << "%PDF-1.7\n";
//     pdfFile << "1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n";
//     pdfFile << "2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n";
//     pdfFile << "3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [0 0 595 842] /Contents 4 0 R /Resources << /Font << /F1 5 0 R >> >> >>\nendobj\n";
//     string content = textStream.str();
//     pdfFile << "4 0 obj\n<< /Length " << content.length() << " >>\nstream\n" << content << "\nendstream\nendobj\n";
//     pdfFile << "5 0 obj\n<< /Type /Font /Subtype /Type1 /BaseFont /Courier >>\nendobj\n";
//     pdfFile << "xref\n0 6\n0000000000 65535 f \n0000000010 00000 n \n0000000065 00000 n \n0000000120 00000 n \n0000000240 00000 n \n0000000420 00000 n \n";
//     pdfFile << "trailer\n<< /Size 6 /Root 1 0 R >>\nstartxref\n500\n%%EOF";

//     pdfFile.close();
//     cout << "(*) PDF Saved to: " << savePath << endl;

//     // ✅ RESET THE WORKING DIRECTORY BACK TO THE ORIGINAL PATH
//     cout << "(*) DEBUG: Resetting working directory back to: " << originalDirectory << endl;
//     fs::current_path(originalDirectory);
// }
void exportMatchHistoryToPDF(const string& filename, int userYear) {
    cout << "Current Working Directory: " << fs::current_path() << endl;

    if (!fs::exists(filename)) {
        cerr << "(!) ERROR: File does not exist: " << fs::absolute(filename) << endl;
        return;
    }

    ifstream historyFile(filename);
    if (!historyFile.is_open()) {
        cerr << "ERROR: Cannot open file: " << filename << endl;
        return;
    }

    #ifdef _WIN32
        string savePath = getSaveFilePath();
        if (savePath.empty()) {
            cout << "Cancelled PDF save.\n";
            historyFile.close();
            return;
        }
    #else
        string savePath = "match_history.pdf";
    #endif

    ofstream pdfFile(savePath, ios::binary);
    if (!pdfFile.is_open()) {
        cerr << "ERROR: Cannot create PDF file: " << savePath << endl;
        historyFile.close();
        return;
    }

    // ✅ Keep PDF Formatting
    stringstream textStream;
    textStream << "BT /F1 12 Tf 50 800 Td (Match History Report for " << userYear << ") Tj ET\n";
    textStream << "BT /F1 10 Tf 50 780 Td (Year: " << userYear << ") Tj ET\n";

    string yearToFind = "Year: " + to_string(userYear);
    bool foundYear = false;
    string line;
    int yPosition = 760;
    int lastYPosition = yPosition; // ✅ Store last printed position
    bool firstSeparatorAdded = false;

    while (getline(historyFile, line)) {
        if (line.find("Year: ") != string::npos) {
            if (line.find(yearToFind) != string::npos) {
                foundYear = true;
                continue;
            } else if (foundYear) {
                break;
            }
        }
        if (foundYear) {
            // ✅ Add the first separator line before the header
            if (!firstSeparatorAdded && line.find("| No.") != string::npos) {
                textStream << "BT /F1 10 Tf 50 " << yPosition << " Td (------------------------------------------------------------------) Tj ET\n";
                yPosition -= 20;
                firstSeparatorAdded = true;
            }

            textStream << "BT /F1 10 Tf 50 " << yPosition << " Td (" << line << ") Tj ET\n";
            lastYPosition = yPosition;  // ✅ Track last position
            yPosition -= 20;
        }
    }

    historyFile.close();

    if (!foundYear) {
        cerr << "(!) ERROR: Year " << userYear << " not found in " << filename << ".\n";
        return;
    }

    // ✅ Extend bottom separator line to match table width
    int bottomLineY = lastYPosition - 30; // Move up 10 units
    string bottomLine = "---------------------------------------------------------------------------------"; // ✅ Longer separator to match table
    textStream << "BT /F1 10 Tf 50 " << bottomLineY << " Td (" << bottomLine << ") Tj ET\n";

    pdfFile << "%PDF-1.7\n";
    pdfFile << "1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n";
    pdfFile << "2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n";
    pdfFile << "3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [0 0 595 842] /Contents 4 0 R /Resources << /Font << /F1 5 0 R >> >> >>\nendobj\n";
    string content = textStream.str();
    pdfFile << "4 0 obj\n<< /Length " << content.length() << " >>\nstream\n" << content << "\nendstream\nendobj\n";
    pdfFile << "5 0 obj\n<< /Type /Font /Subtype /Type1 /BaseFont /Courier >>\nendobj\n";
    pdfFile << "xref\n0 6\n0000000000 65535 f \n0000000010 00000 n \n0000000065 00000 n \n0000000120 00000 n \n0000000240 00000 n \n0000000420 00000 n \n";
    pdfFile << "trailer\n<< /Size 6 /Root 1 0 R >>\nstartxref\n500\n%%EOF";

    pdfFile.close();
    cout << "(*) PDF Saved to: " << savePath << endl;

    // ✅ Reset Working Directory
    cout << "(*) DEBUG: Resetting working directory back to: " << originalDirectory << endl;
    fs::current_path(originalDirectory);
}


    #ifdef _WIN32
    string getSaveFilePath() {
        char filename[MAX_PATH] = {0};
    
        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = "PDF Files (*.pdf)\0*.pdf\0All Files (*.*)\0*.*\0";
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOREADONLYRETURN | OFN_CREATEPROMPT;
        ofn.lpstrDefExt = "pdf";
    
        if (GetSaveFileName(&ofn)) {
            string filePath = string(filename);
    
            // ✅ Ensure the file has a .pdf extension
            if (filePath.find(".pdf") == string::npos) {
                filePath += ".pdf";  // Append .pdf if missing
            }
    
            return filePath;
        }
        return ""; // Return empty string if user cancels
    }
    #endif

