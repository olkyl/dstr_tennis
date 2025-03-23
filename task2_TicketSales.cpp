// KOO_BOH_CHUN TP065141
#include "task2_TicketSales.hpp"

Ticket::Ticket() : customerName(""), priority(0) {}

Ticket::Ticket(string customerName, int priority)
{
    this->customerName = customerName;
    this->priority = priority;
}

Queue::Queue(int s)
{
    front = rear = -1;
    size = s;
    Q = new Ticket[size];
}

Queue::~Queue()
{
    delete[] Q;
}

// Check whether the queue is empty
bool Queue::isEmpty()
{
    return front == -1 || front > rear;
}

// Insert Element inside the queue
void Queue::enqueue(Ticket t)
{
    if (rear == size - 1)
    {
        cout << "Queue Full" << endl;
    }
    else
    {
        if (front == -1)
            front = 0;
        rear++;
        Q[rear] = t;
    }
}

// Remove Element from the queue
Ticket Queue::dequeue()
{
    if (isEmpty())
    {
        cout << "Queue is Empty" << endl;
        return Ticket();
    }
    else
    {
        Ticket temp = Q[front];
        front++;
        if (front > rear)
        {
            front = rear = -1;
        }
        return temp;
    }
}

// Display element inside the queue
void Queue::display()
{
    if (!isEmpty())
    {
        for (int i = front; i <= rear; i++)
        {
            cout << "Customer: " << Q[i].customerName << " | Priority: " << Q[i].priority << endl;
        }
    }
    else
    {
        cout << "This queue is empty." << endl;
    }
    cout << "\n";
}

// Process Ticket Sales Order: VIP -> Early-Bird -> Public Access
void processTicket(Queue &vip, Queue &earlyBird, Queue &normal, ofstream &file)
{
    cout << "--- Sales Ticket Processing ---" << endl;
    file << "\n--- Sales Ticket Processing ---" << endl;

    while (!vip.isEmpty())
    {
        Ticket t = vip.dequeue();
        cout << "Ticket sold to: " << t.customerName << " (VIP)" << endl;
        file << "Ticket sold to: " << t.customerName << " (VIP)" << endl;
    }

    while (!earlyBird.isEmpty())
    {
        Ticket t = earlyBird.dequeue();
        cout << "Ticket sold to: " << t.customerName << " (Early-Bird)" << endl;
        file << "Ticket sold to: " << t.customerName << " (Early-Bird)" << endl;
    }

    while (!normal.isEmpty())
    {
        Ticket t = normal.dequeue();
        cout << "Ticket sold to: " << t.customerName << " (Public Access)" << endl;
        file << "Ticket sold to: " << t.customerName << " (Public Access)" << endl;
    }
}

// Entrance Order: Public Access (Need come early to queue, no special lane, take times to settle) -> Early-Bird (Got special lane) -> VIP (Got reserved seat & special lane no need come early)
void processEntry(Queue &vip, Queue &earlyBird, Queue &normal, ofstream &file)
{
    cout << "\n--- Spectator Entry Process ---" << endl;
    file << "\n--- Spectator Entry Process ---" << endl;

    while (!normal.isEmpty())
    {
        Ticket t = normal.dequeue();
        cout << t.customerName << " (Public Access) has entered the venue." << endl;
        file << t.customerName << " (Public Access) has entered the venue." << endl;
    }

    while (!earlyBird.isEmpty())
    {
        Ticket t = earlyBird.dequeue();
        cout << t.customerName << " (Early-Bird) has entered the venue." << endl;
        file << t.customerName << " (Early-Bird) has entered the venue." << endl;
    }

    while (!vip.isEmpty())
    {
        Ticket t = vip.dequeue();
        cout << t.customerName << " (VIP) has entered the venue." << endl;
        file << t.customerName << " (VIP) has entered the venue." << endl;
    }
}

// Exit Order: VIP -> Early-Bird -> Public Access
void processExit(Queue &vipExit, Queue &earlyBirdExit, Queue &normalExit, ofstream &file)
{
    cout << "\n--- Spectator Exit Process ---" << endl;
    file << "\n--- Spectator Exit Process ---" << endl;

    while (!vipExit.isEmpty())
    {
        Ticket t = vipExit.dequeue();
        cout << t.customerName << " (VIP) has exited the venue." << endl;
        file << t.customerName << " (VIP) has exited the venue." << endl;
    }

    while (!earlyBirdExit.isEmpty())
    {
        Ticket t = earlyBirdExit.dequeue();
        cout << t.customerName << " (Early-Bird) has exited the venue." << endl;
        file << t.customerName << " (Early-Bird) has exited the venue." << endl;
    }

    while (!normalExit.isEmpty())
    {
        Ticket t = normalExit.dequeue();
        cout << t.customerName << " (Public Access) has exited the venue." << endl;
        file << t.customerName << " (Public Access) has exited the venue." << endl;
    }

    file << "\n";
}

// Main Function
void ticketSales()
{
    // Create and open a text file
    ofstream MyFile("ticketSales&SpectatorManagement.txt", ios::app);

    // Get Current Date & Time
    time_t timestamp;
    time(&timestamp);
    MyFile << "===================================" << endl;
    MyFile << "DateTime: " << ctime(&timestamp);
    MyFile << "===================================" << endl;

    // Ask for user input and save into an array
    int numberOfPeople;
    cout << "\nHow many people in the queue: ";
    cin >> numberOfPeople;
    cin.ignore();

    Ticket ticketArray[numberOfPeople];
    for (int i = 0; i < numberOfPeople; i++)
    {
        string clientName;
        int priorityLevel;

        cout << "\nCustomer " << (i + 1) << " Name: ";
        getline(cin, clientName);

        cout << "Customer Type (VIP=3, Early-bird=2, Public Access=1): ";
        cin >> priorityLevel;
        cin.ignore();

        ticketArray[i] = Ticket(clientName, priorityLevel);
    }

    // Display initial ticket queue (Meaning hasn't perform priority queue)
    cout << "\n--- Initial Ticket Queue ---" << endl;
    // Write to text file
    MyFile << "--- Initial Ticket Queue ---" << endl;
    for (const auto &ticket : ticketArray)
    {
        string type;
        switch (ticket.priority)
        {
        case 1:
            type = "Public Access";
            break;
        case 2:
            type = "Early-bird";
            break;
        case 3:
            type = "VIP";
            break;
        }
        cout << "Customer: " << ticket.customerName << " (" << type << ")\n";
        // Write to text file
        MyFile << "Customer: " << ticket.customerName << " (" << type << ")\n";
    }

    // Count queue sizes
    int lPASize = 0, mPASize = 0, hPASize = 0;
    for (const auto &ticket : ticketArray)
    {
        switch (ticket.priority)
        {
        case 1:
            lPASize++;
            break;
        case 2:
            mPASize++;
            break;
        case 3:
            hPASize++;
            break;
        }
    }

    // Creating the queue
    Queue vipQueue(hPASize), earlyBirdQueue(mPASize), normalQueue(lPASize);
    Queue vipQueueCopy(hPASize), earlyBirdQueueCopy(mPASize), normalQueueCopy(lPASize);
    Queue vipExit(hPASize), earlyBirdExit(mPASize), normalExit(lPASize);

    // Start Insert element inside their respective queue based on the priority level (Normal=1, Early-Bird=2, VIP=3)
    for (const auto &ticket : ticketArray)
    {
        switch (ticket.priority)
        {
        case 1:
            normalQueue.enqueue(ticket);
            normalQueueCopy.enqueue(ticket);
            normalExit.enqueue(ticket);
            break;
        case 2:
            earlyBirdQueue.enqueue(ticket);
            earlyBirdQueueCopy.enqueue(ticket);
            earlyBirdExit.enqueue(ticket);
            break;
        case 3:
            vipQueue.enqueue(ticket);
            vipQueueCopy.enqueue(ticket);
            vipExit.enqueue(ticket);
            break;
        }
    }

    // Display element in each queue
    cout << "\nTicket Sorted Into Their Respective Priority Queue: " << endl;
    cout << "\n1.VIP Queue" << endl;
    vipQueue.display();
    cout << "2.Early-Bird Queue" << endl;
    earlyBirdQueue.display();
    cout << "3.Public Access Queue" << endl;
    normalQueue.display();

    processTicket(vipQueue, earlyBirdQueue, normalQueue, MyFile);
    processEntry(vipQueueCopy, earlyBirdQueueCopy, normalQueueCopy, MyFile);
    processExit(vipExit, earlyBirdExit, normalExit, MyFile);

    // Close the file
    MyFile.close();
    filesystem::path cwd = filesystem::current_path();
    cout << "\nData been saved to " << cwd.string() << "\\ticketSales&SpectatorManagement.txt" << endl;
}
