#include "allStructs.hpp"
#include "task1_schedulingSystem.hpp"
#include "task1_playerManager.hpp"

// Function to load players from CSV file directly into a queue
void loadPlayersToQueue(const string& filename, PlayersQueue& playerQueue, int targetYear) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "(!) Error: Could not open file " << filename << endl;
        return;
    }

    // Check for and skip BOM if present
    char bom[3];
    file.read(bom, 3);
    if (!(bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF)) {
        file.seekg(0); // If no BOM, reset file pointer to beginning
    }
    
    string line;
    int playerCount = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        
        Player* newPlayer = new Player();
        
        // Parse playerID
        getline(ss, token, ',');
        newPlayer->playerID = token;
        
        // Parse playerName
        getline(ss, token, ',');
        newPlayer->playerName = token;
        
        // Parse gender
        getline(ss, token, ',');
        newPlayer->gender = token[0];
        
        // Parse school
        getline(ss, token, ',');
        newPlayer->school = token;
        
        // Parse rank
        getline(ss, token, ',');
        newPlayer->rank = stoi(token);

        // Parse year 
        int year = 0;
        if (getline(ss, token, ',')) {
            year = stoi(token);
        }
        
        newPlayer->next = nullptr;
        
        // Only add the player if they match the target year
        if (year == targetYear) {
            playerQueue.enqueue(newPlayer); // Add to queue using rank as priority
            playerCount++;
        } else {
            delete newPlayer; // Skip this player (not from the target year)
        }
    }
    
    file.close();
    cout << "(*) Players loaded from file to queue." << endl;
}

// Function to display all players in a queue 
void displayPlayerQueue(PlayersQueue& playerQueue) {
    if (playerQueue.isEmpty()) {
        cout << "(#) No players in queue yet." << endl;
        return;
    }

    cout << "----------------------------------------------------------" << endl;
    cout << "ID   | Name             | Gender | School | Rank" << endl;
    cout << "----------------------------------------------------------" << endl;

    // Use a temporary queue to store players while displaying
    PlayersQueue tempQueue;
    int count = 0;

    while (!playerQueue.isEmpty()) {
        Player* player = playerQueue.dequeue();
        printf("%-5s| %-17s| %-7c| %-7s| %-4d\n", 
               player->playerID.c_str(), 
               player->playerName.c_str(), 
               player->gender, 
               player->school.c_str(), 
               player->rank);
        tempQueue.enqueue(player, player->rank);
        count++;
    }

    cout << "----------------------------------------------------------" << endl;
    cout << "Displayed " << count << " players. Restoring queue..." << endl;

    // Restore original order
    while (!tempQueue.isEmpty()) {
        playerQueue.enqueue(tempQueue.dequeue());
    }

    // Debugging purpose
    // cout << "Queue restored. Total players: " << playerQueue.size() << endl;
}

// Function to free memory for players in a queue
void freePlayerQueue(PlayersQueue& playerQueue) {
    while (!playerQueue.isEmpty()) {
        Player* player = playerQueue.dequeue();
        delete player;
    }
    cout << "(*) Player queue memory freed." << endl;
}
