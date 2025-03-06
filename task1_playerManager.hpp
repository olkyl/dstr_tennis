#ifndef PLAYERMANAGEMENT_HPP
#define PLAYERMANAGEMENT_HPP

#include "allStructs.hpp"
#include <string>

using namespace std;

// Players class - priority queue based on each player's rank
class PlayersQueue {
private:
    struct Node {
        Player* data;
        int priority;
        Node* next;
    };
    Node* front;
    int count;

public:
    PlayersQueue() {
        front = nullptr;
        count = 0;
    }
    
    ~PlayersQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Enqueue with priority (lower rank number = higher priority, 0 = lowest priority)
    void enqueue(Player* player, int priority) {
        Node* newNode = new Node;
        newNode->data = player;
        newNode->priority = priority;
        
        // Special case: If priority is 0 (no rank), treat it as lowest priority
        if (priority == 0) {
            // Put at the end of the queue
            if (isEmpty()) {
                newNode->next = nullptr;
                front = newNode;
            } else {
                Node* temp = front;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                newNode->next = nullptr;
                temp->next = newNode;
            }
        }
        // Normal case: If queue is empty or new node has higher priority, insert at front
        else if (isEmpty() || (priority < front->priority && front->priority != 0) || front->priority == 0) {
            newNode->next = front;
            front = newNode;
        } else {
            Node* temp = front;
            while (temp->next != nullptr && 
                ((temp->next->priority <= priority && temp->next->priority != 0) || priority == 0)) {
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
        count++;
    }
    
    // Calls the previous enqueue(Player* player, int priority) method and uses player's rank as priority
    void enqueue(Player* player) {
        enqueue(player, player->rank);
    }
    
    Player* dequeue() {
        if (isEmpty()) {
            throw runtime_error("(!) PlayersQueue is empty");
        }
        
        Player* player = front->data;
        Node* temp = front;
        front = front->next;
        delete temp;
        count--;
        return player;
    }
    
    // not used, might delete
    Player* peek() const {
        if (isEmpty()) {
            throw runtime_error("(!) PlayersQueue is empty");
        }
        return front->data;
    }
    
    bool isEmpty() const {
        return front == nullptr;
    }
    
    int size() const {
        return count;
    }
};
   
// Player class - circular queue for players who will participate in round robin
class CircularPlayersQueue {
private:
    struct Node {
        Player* data;
        Node* next;
    };
    Node* head;
    Node* tail;
    int count;

public:
    CircularPlayersQueue() {
        head = nullptr;
        tail = nullptr;
        count = 0;
    }
    
    ~CircularPlayersQueue() {
        clear();
    }

    void clear() {
        while (!isEmpty()) {
            dequeue(); // Remove all nodes safely
        }
    }
    
    void enqueue(Player* player) {
        Node* newNode = new Node;
        newNode->data = player;

        if (isEmpty()) {
            head = newNode;
            tail = newNode;
            newNode->next = head; // Circular link
        } else {
            tail->next = newNode;
            newNode->next = head; // Maintain circular link
            tail = newNode; // Update tail
        }
        count++;
    }

    Player* dequeue() {
        if (isEmpty()) {
            throw runtime_error("(!) CircularPlayersQueue is empty");
        }

        Player* removedPlayer = head->data;
        if (count == 1) {
            delete head;
            head = nullptr;
            tail = nullptr;
        } else {
            Node* temp = head;
            head = head->next;
            tail->next = head; // Maintain circular link
            delete temp;
        }
        count--;
        return removedPlayer;
    }

    void rotate() {
        if (count > 1) {
            head = head->next;
            tail = tail->next;
        }
    }
    
    Player* peek(int position) const {
        if (isEmpty() || position >= count) {
            throw runtime_error("(!) Invalid position in CircularPlayersQueue");
        }

        Node* current = head;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }

        return current->data;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    int size() const {
        return count;
    }
};    

// function prototypes
void loadPlayersToQueue(const string& filename, PlayersQueue& playerQueue);
void displayPlayerQueue(PlayersQueue& playerQueue);
void freePlayerQueue(PlayersQueue& playerQueue);

#endif // PLAYER_MANAGEMENT_HPP