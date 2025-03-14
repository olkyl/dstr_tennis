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
   
// function prototypes
void loadPlayersToQueue(const string& filename, PlayersQueue& playerQueue, int targetYear);
void displayPlayerQueue(PlayersQueue& playerQueue);
void freePlayerQueue(PlayersQueue& playerQueue);

#endif // PLAYER_MANAGEMENT_HPP