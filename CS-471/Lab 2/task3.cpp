#include <iostream>
#include <deque>
#include <string>

using namespace std;

// Task structure
struct Task {
    string description;
    bool isHighPriority;

    Task(string desc, bool priority) : description(desc), isHighPriority(priority) {}
};

// Task Scheduling System class
class TaskScheduler {
private:
    deque<Task> taskQueue;

public:
    // Insert task at the front (for high-priority tasks)
    void addTaskAtFront(const string& description) {
        taskQueue.push_front(Task(description, true));
        cout << "Added high-priority task: " << description << " at the front" << endl;
    }

    // Insert task at the back (for regular tasks)
    void addTaskAtBack(const string& description) {
        taskQueue.push_back(Task(description, false));
        cout << "Added regular task: " << description << " at the back" << endl;
    }

    // Remove task from the front
    void removeTaskFromFront() {
        if (taskQueue.empty()) {
            cout << "No tasks to remove from the front." << endl;
        } else {
            cout << "Removed task from the front: " << taskQueue.front().description << endl;
            taskQueue.pop_front();
        }
    }

    // Remove task from the back
    void removeTaskFromBack() {
        if (taskQueue.empty()) {
            cout << "No tasks to remove from the back." << endl;
        } else {
            cout << "Removed task from the back: " << taskQueue.back().description << endl;
            taskQueue.pop_back();
        }
    }

    // Access the task at the front
    void accessTaskAtFront() {
        if (taskQueue.empty()) {
            cout << "No tasks at the front." << endl;
        } else {
            cout << "Task at the front: " << taskQueue.front().description
                 << " (Priority: " << (taskQueue.front().isHighPriority ? "High" : "Regular") << ")" << endl;
        }
    }

    // Access the task at the back
    void accessTaskAtBack() {
        if (taskQueue.empty()) {
            cout << "No tasks at the back." << endl;
        } else {
            cout << "Task at the back: " << taskQueue.back().description
                 << " (Priority: " << (taskQueue.back().isHighPriority ? "High" : "Regular") << ")" << endl;
        }
    }

    // Display all tasks in the deque
    void displayAllTasks() {
        if (taskQueue.empty()) {
            cout << "No tasks in the queue." << endl;
            return;
        }

        cout << "Tasks in the queue:" << endl;
        for (const auto& task : taskQueue) {
            cout << "- " << task.description
                 << " (Priority: " << (task.isHighPriority ? "High" : "Regular") << ")" << endl;
        }
    }
};

// Main function to demonstrate task scheduling operations
int main() {
    TaskScheduler scheduler;

    // Add tasks
    scheduler.addTaskAtBack("Task 1: Regular Task");
    scheduler.addTaskAtFront("Task 2: High Priority Task");
    scheduler.addTaskAtBack("Task 3: Regular Task");
    scheduler.addTaskAtFront("Task 4: High Priority Task");

    // Display all tasks
    scheduler.displayAllTasks();

    // Access tasks at both ends
    scheduler.accessTaskAtFront();
    scheduler.accessTaskAtBack();

    // Remove tasks from both ends
    scheduler.removeTaskFromFront();
    scheduler.removeTaskFromBack();

    // Display all tasks after removals
    scheduler.displayAllTasks();

    return 0;
}
