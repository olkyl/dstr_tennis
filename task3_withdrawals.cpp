#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Player {
    int playerID;
    string playerName;
    Player* next;
    Player(int id, string name) : playerID(id), playerName(name), next(nullptr) {}
};

struct WithdrawnPlayer {
    int playerID;
    string playerName;
    string reason;
    WithdrawnPlayer* next;
    WithdrawnPlayer(int id, string name, string reason)
        : playerID(id), playerName(name), reason(reason), next(nullptr) {}
};

class Queue {
private:
    Player* front;
    Player* rear;
public:
    Queue() : front(nullptr), rear(nullptr) {}
    bool isEmpty() { return front == nullptr; }
    void enqueue(int id, string name) {
        Player* newPlayer = new Player(id, name);
        if (isEmpty()) {
            front = rear = newPlayer;
        } else {
            rear->next = newPlayer;
            rear = newPlayer;
        }
    }
    Player* dequeue() {
        if (isEmpty()) return nullptr;
        Player* temp = front;
        front = front->next;
        if (!front) rear = nullptr;
        return temp;
    }
};

class WithdrawnPlayersStack {
private:
    WithdrawnPlayer* top;
public:
    WithdrawnPlayersStack() : top(nullptr) {}
    bool isEmpty() { return top == nullptr; }
    void push(int id, string name, string reason) {
        WithdrawnPlayer* newWithdrawn = new WithdrawnPlayer(id, name, reason);
        newWithdrawn->next = top;
        top = newWithdrawn;
    }
    WithdrawnPlayer* pop() {
        if (isEmpty()) return nullptr;
        WithdrawnPlayer* temp = top;
        top = top->next;
        return temp;
    }
    void display() const {
        if (isEmpty()) {
            cout << "No players have withdrawn." << endl;
            return;
        }
        WithdrawnPlayer* current = top;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << "Reason" << endl;
        cout << string(50, '-') << endl;
        while (current) {
            cout << left << setw(10) << current->playerID
                 << setw(20) << current->playerName
                 << current->reason << endl;
            current = current->next;
        }
    }
};

void withdrawPlayer(Queue& allPlayersQueue, WithdrawnPlayersStack& withdrawnStack) {
    int playerID;
    string reason;
    cout << "Enter Player ID to withdraw: ";
    cin >> playerID;
    cout << "Enter withdrawal reason: ";
    cin.ignore();
    getline(cin, reason);
    
    Queue tempQueue;
    bool found = false;
    while (!allPlayersQueue.isEmpty()) {
        Player* player = allPlayersQueue.dequeue();
        if (player->playerID == playerID) {
            withdrawnStack.push(player->playerID, player->playerName, reason);
            delete player;
            found = true;
        } else {
            tempQueue.enqueue(player->playerID, player->playerName);
        }
    }
    while (!tempQueue.isEmpty()) {
        Player* temp = tempQueue.dequeue();
        allPlayersQueue.enqueue(temp->playerID, temp->playerName);
        delete temp;
    }
    if (!found) {
        cout << "Player not found!" << endl;
    } else {
        cout << "Player successfully withdrawn." << endl;
    }
}

void substitutePlayer(WithdrawnPlayersStack& withdrawnStack, Queue& eliminatedPlayers) {
    if (withdrawnStack.isEmpty()) {
        cout << "No withdrawn players available." << endl;
        return;
    }
    if (eliminatedPlayers.isEmpty()) {
        cout << "No substitute players available." << endl;
        return;
    }
    WithdrawnPlayer* withdrawnPlayer = withdrawnStack.pop();
    Player* substitute = eliminatedPlayers.dequeue();
    if (!substitute) {
        cout << "Error: No available substitutes." << endl;
        delete withdrawnPlayer;
        return;
    }
    cout << "Substituted " << withdrawnPlayer->playerName << " with " << substitute->playerName << " successfully." << endl;
    delete withdrawnPlayer;
}

void displayWithdrawalMenu() {
    cout << "\nWithdrawal Menu:\n";
    cout << "1. Withdraw Player\n";
    cout << "2. Substitute Withdrawn Player\n";
    cout << "3. Display Withdrawn Players\n";
    cout << "4. Exit\n";
    cout << "Enter choice: ";
}

int getChoice(int maxOption) {
    int choice;
    cin >> choice;
    while (choice < 1 || choice > maxOption) {
        cout << "Invalid choice. Enter again: ";
        cin >> choice;
    }
    return choice;
}

void handlePlayerWithdrawals(Queue& allPlayersQueue, WithdrawnPlayersStack& withdrawnStack, Queue& eliminatedPlayers) {
    int choice;
    do {
        displayWithdrawalMenu();
        choice = getChoice(4);
        switch (choice) {
            case 1:
                withdrawPlayer(allPlayersQueue, withdrawnStack);
                break;
            case 2:
                substitutePlayer(withdrawnStack, eliminatedPlayers);
                break;
            case 3:
                withdrawnStack.display();
                break;
            case 4:
                cout << "Exiting withdrawal management." << endl;
                break;
        }
    } while (choice != 4);
}

int main() {
    Queue allPlayersQueue, eliminatedPlayers;
    WithdrawnPlayersStack withdrawnStack;
    allPlayersQueue.enqueue(1, "Alice");
    allPlayersQueue.enqueue(2, "Bob");
    allPlayersQueue.enqueue(3, "Charlie");
    eliminatedPlayers.enqueue(4, "Dave");
    eliminatedPlayers.enqueue(5, "Eve");
    handlePlayerWithdrawals(allPlayersQueue, withdrawnStack, eliminatedPlayers);
    return 0;
}
