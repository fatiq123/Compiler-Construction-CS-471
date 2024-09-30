#include <iostream>
#include <queue>
#include <deque>
#include <string>

using namespace std;

class TicketingSystem {
private:
    queue<string> regularQueue;    // Regular customers queue
    deque<string> vipQueue;        // VIP customers deque

public:
    // Function to add a regular customer to the queue
    void joinQueue(const string& customer) {
        regularQueue.push(customer);
        cout << customer << " has joined the regular queue." << endl;
    }

    // Function to add a VIP customer (VIPs are served at the front)
    void addVIP(const string& vipCustomer) {
        vipQueue.push_back(vipCustomer); // VIPs are added to the back of the deque
        cout << vipCustomer << " has joined the VIP queue." << endl;
    }

    // Function to process the next customer in the queue
    void processTicket() {
        if (!vipQueue.empty()) {
            // Serve the next VIP customer
            string vipCustomer = vipQueue.front();
            vipQueue.pop_front();
            cout << "Processing ticket for VIP customer: " << vipCustomer << endl;
        } else if (!regularQueue.empty()) {
            // Serve the next regular customer
            string customer = regularQueue.front();
            regularQueue.pop();
            cout << "Processing ticket for regular customer: " << customer << endl;
        } else {
            // No customers in the queue
            cout << "No customers in the queue to process." << endl;
        }
    }

    // Function to display the current queue status
    void displayQueueStatus() {
        cout << "\nCurrent Queue Status:" << endl;

        // Display VIP queue
        if (!vipQueue.empty()) {
            cout << "VIP Queue: ";
            for (const string& vip : vipQueue) {
                cout << vip << " ";
            }
            cout << endl;
        } else {
            cout << "VIP Queue: Empty" << endl;
        }

        // Display regular queue
        if (!regularQueue.empty()) {
            cout << "Regular Queue: ";
            queue<string> tempQueue = regularQueue; // Temporary copy for iteration
            while (!tempQueue.empty()) {
                cout << tempQueue.front() << " ";
                tempQueue.pop();
            }
            cout << endl;
        } else {
            cout << "Regular Queue: Empty" << endl;
        }
        cout << endl;
    }
};

// Main function to simulate the ticketing system
int main() {
    TicketingSystem cinemaQueue;

    // Adding regular customers to the queue
    cinemaQueue.joinQueue("Customer 1");
    cinemaQueue.joinQueue("Customer 2");

    // Adding VIP customers
    cinemaQueue.addVIP("VIP 1");

    // Display queue status
    cinemaQueue.displayQueueStatus();

    // Process tickets
    cinemaQueue.processTicket();  // Should process VIP 1
    cinemaQueue.processTicket();  // Should process Customer 1

    // Adding another VIP customer
    cinemaQueue.addVIP("VIP 2");

    // Process tickets
    cinemaQueue.processTicket();  // Should process VIP 2
    cinemaQueue.processTicket();  // Should process Customer 2

    // Display final queue status
    cinemaQueue.displayQueueStatus();

    return 0;
}
