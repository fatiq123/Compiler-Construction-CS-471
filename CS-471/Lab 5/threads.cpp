#include <iostream>
#include <thread>

void printMessage(int count, const std::string &message) {
    for (int i = 0; i < count; ++i) {
        std::cout << message << " - " << i << std::endl;
    }
}

int main() {

    // Create threads
    std::thread t1(task1);  // Launch task1 in a separate thread
    std::thread t2(task2);  // Launch task2 in a separate thread

    // Wait for threads to finish
    t1.join();  // Ensure t1 finishes before main thread continues
    t2.join();  // Ensure t2 finishes before main thread continues

    std::cout << "Both tasks completed!" << std::endl;









    // Launch a thread using a lambda function
    std::thread t([] {
        for (int i = 0; i < 5; ++i) {
            std::cout << "Lambda thread - count: " << i << std::endl;
        }
    });

    t.join();  // Wait for the thread to finish

    std::cout << "Lambda task completed!" << std::endl;







    std::thread t(printMessage, 5, "Hello from thread");

    t.join();  // Wait for thread to finish

    return 0;
}




void task1() {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Task 1 - count: " << i << std::endl;
    }
}

void task2() {
    for (int i = 0; i < 5; ++i) {
        std::cout << "Task 2 - count: " << i << std::endl;
    }
}






void backgroundTask() {
    std::this_thread::sleep_for(std::chrono::seconds(2));  // Simulate long task
    std::cout << "Background task finished!" << std::endl;
}
