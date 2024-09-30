#include <iostream>
#include <string>

using namespace std;

// Node structure for doubly linked list
struct Node {
    string url;
    Node* prev;
    Node* next;

    Node(string newUrl) : url(newUrl), prev(nullptr), next(nullptr) {}
};

// Doubly Linked List to manage browser history
class BrowserHistory {
private:
    Node* head;
    Node* current;

public:
    BrowserHistory() : head(nullptr), current(nullptr) {}

    // Add a new page (URL) to the browser history
    void addPage(const string& url) {
        Node* newNode = new Node(url);
        if (!head) {  // If list is empty
            head = newNode;
            current = head;
        } else {
            newNode->prev = current;
            current->next = newNode;
            current = newNode;
        }
        cout << "Page added: " << url << endl;
    }

    // Move backward in history
    void moveBackward() {
        if (current && current->prev) {
            current = current->prev;
            cout << "Moved back to: " << current->url << endl;
        } else {
            cout << "No previous page." << endl;
        }
    }

    // Move forward in history
    void moveForward() {
        if (current && current->next) {
            current = current->next;
            cout << "Moved forward to: " << current->url << endl;
        } else {
            cout << "No next page." << endl;
        }
    }

    // Delete the current page from the history
    void deletePage() {
        if (!current) {
            cout << "No page to delete." << endl;
            return;
        }
        cout << "Deleting page: " << current->url << endl;
        Node* temp = current;

        if (current->prev) {
            current->prev->next = current->next;
        } else {  // If deleting the head
            head = current->next;
        }

        if (current->next) {
            current->next->prev = current->prev;
            current = current->next;  // Move forward after deletion
        } else {  // If it's the last page, move back
            current = current->prev;
        }

        delete temp;
    }

    // Display the current page
    void displayCurrentPage() {
        if (current) {
            cout << "Current page: " << current->url << endl;
        } else {
            cout << "No page loaded." << endl;
        }
    }
};

int main() {
    BrowserHistory browser;

    // Add pages to the browser history
    browser.addPage("https://www.example.com");
    browser.addPage("https://www.google.com");
    browser.addPage("https://www.wikipedia.org");

    // Navigate through history
    browser.displayCurrentPage();
    browser.moveBackward();
    browser.displayCurrentPage();
    browser.moveBackward();
    browser.displayCurrentPage();
    browser.moveForward();
    browser.displayCurrentPage();

    // Delete a page
    browser.deletePage();
    browser.displayCurrentPage();

    return 0;
}
