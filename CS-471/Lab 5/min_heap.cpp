#include <iostream>
#include <queue>
#include <vector>  // Required for std::vector

int main() {
    // Min-heap using std::greater
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    // Insert elements
    minHeap.push(10);
    minHeap.push(30);
    minHeap.push(20);
    minHeap.push(5);

    // Display the smallest element
    std::cout << "Top element (min-heap): " << minHeap.top() << std::endl;  // Output: 5

    return 0;
}


// Custom comparator for a min-heap based on the second element of a pair
struct Compare {
    bool operator()(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
        return p1.second > p2.second;  // Compare based on second element
    }
};

int main() {
    // Priority queue of pairs with a custom comparator
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, Compare> pq;

    // Insert pairs into the priority queue
    pq.push({1, 10});
    pq.push({2, 5});
    pq.push({3, 20});

    // Display the pair with the highest priority (based on the second element)
    std::cout << "Top element (based on second value): {" << pq.top().first << ", " << pq.top().second << "}" << std::endl;

    return 0;
}

#include <iostream>
using namespace std;

template <typename T>
T add(T a, T b) {
    return a + b;
}

int main() {
    cout << "Sum of integers: " << add(3, 7) << endl;        // Works with integers
    cout << "Sum of floats: " << add(3.5, 7.2) << endl;      // Works with floats
    return 0;
}

#include <iostream>
using namespace std;

template <typename T>
class Box {
    T value;
public:
    Box(T val) : value(val) {}

    void showValue() {
        cout << "Value: " << value << endl;
    }
};

int main() {
    Box<int> intBox(10);        // Box for integers
    intBox.showValue();

    Box<double> doubleBox(10.5);  // Box for doubles
    doubleBox.showValue();

    return 0;
}
