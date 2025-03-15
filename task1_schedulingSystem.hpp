#ifndef SCHEDULINGSYSTEM_HPP
#define SCHEDULINGSYSTEM_HPP

#include "allStructs.hpp"
#include "task1_playerManager.hpp"
#include <string>

using namespace std;

extern PlayersQueue eliminatedPlayers; // Declare the queue as external

// Match class - regular queue in order of creation
class MatchesQueue {
private:
    struct Node {
        Match* data;
        Node* next;
    };
    Node* front;
    Node* rear;
    int count;

public:
    MatchesQueue() {
        front = nullptr;
        rear = nullptr;
        count = 0;
    }
    
    ~MatchesQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    // Enqueue a match to the end of the queue
    void enqueue(Match* match) {
        Node* newNode = new Node;
        newNode->data = match;
        newNode->next = nullptr;
        
        // If queue is empty, front and rear both point to the new node
        if (isEmpty()) {
            front = newNode;
            rear = newNode;
        } else {
            // Add new node at the end and update rear
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }
    
    // Dequeue a match from the front of the queue
    Match* dequeue() {
        if (isEmpty()) {
            throw runtime_error("(!) MatchesQueue is empty");
        }
        
        Match* match = front->data;
        Node* temp = front;
        front = front->next;
        
        // If queue becomes empty after dequeue
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        count--;
        return match;
    }

    // Peek at the front match without removing it
    Match* peek() const {
        if (isEmpty()) {
            throw runtime_error("(!) MatchesQueue is empty");
        }
        return front->data;
    }
    
    // Check if queue is empty
    bool isEmpty() const {
        return front == nullptr;
    }
    
    // Get number of matches in the queue
    int size() const {
        return count;
    }

    // Get the front of the queue
    Node* head() const {
        return front; // by returning the head node
    }
};


// Function prototypes
string getNextDate(const string& date);
string incrementTime(const string& time, int minutesToAdd);
string generateMatchID(const string& matchType);
void generateScores(Match* match);
void freeMatchesQueue(MatchesQueue& matchesQueue);
void displayMatches(MatchesQueue& matchesQueue);
void createMatches_QF(PlayersQueue& playerQueue, MatchesQueue& matchQueue, int year);
void getResults_QF(MatchesQueue& matchQueue, PlayersQueue& playersQueue, PlayersQueue& winnersQueue);
void createMatches_RR(PlayersQueue& qualifiedPlayers, MatchesQueue& matchQueue, int year);
void getResults_RR(MatchesQueue& matchQueue, PlayersQueue& playersQueue, PlayersQueue& winnersQueue);
void createMatches_KO(PlayersQueue& playerQueue, MatchesQueue& matchQueue, int year);
void getResults_KO(MatchesQueue& matchQueue, PlayersQueue& playersQueue, PlayersQueue& winnersQueue);
void simulatePastTournament(const string& csv_filename, int year);

#endif // SCHEDULING_SYSTEM_HPP