// ticket.hpp
#ifndef TICKET_HPP
#define TICKET_HPP

#include <iostream>
#include <fstream>
#include <ctime>
#include <filesystem>
#include <string>
using namespace std;

// Ticket Class
class Ticket
{
public:
    string customerName;
    int priority;

    Ticket();
    Ticket(string customerName, int priority);
};

// Queue Class
class Queue
{
private:
    int front, rear, size;
    Ticket *Q;

public:
    Queue(int s);
    ~Queue();

    bool isEmpty();
    void enqueue(Ticket t);
    Ticket dequeue();
    void display();
};

// Utility Functions
void processTicket(Queue &vip, Queue &earlyBird, Queue &normal);
void processEntry(Queue &vip, Queue &earlyBird, Queue &normal);
void processExit(Queue &vipExit, Queue &earlyBirdExit, Queue &normalExit);
void ticketSales();

#endif // TICKET_HPP
