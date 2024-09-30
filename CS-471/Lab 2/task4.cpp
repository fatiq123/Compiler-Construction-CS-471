#include <iostream>
#include <stack>
#include <string>
using namespace std;

bool isBalanced(const string& expression) {
    
    stack<char> s;

    for (char ch : expression) {
        
        if (ch == '(' || ch == '{' || ch == '[') {
            s.push(ch);
        }
        else if (ch == ')' || ch == '}' || ch == ']') {

            if (s.empty()) {
                return false;
            }

            // Pop the top element from the stack and check if it matches
            char top = s.top();
            s.pop();

            // Check if the closing bracket matches the top opening bracket
            if ((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '[')) {
                return false;
            }
        }
    }

    // After processing the expression, if the stack is empty, it means all brackets were matched
    return s.empty();
}

int main() {
    string expression;

    cout << "Enter a mathematical expression: ";
    cin >> expression;

    if (isBalanced(expression)) {
        cout << "The expression has balanced parentheses." << endl;
    } else {
        cout << "The expression does not have balanced parentheses." << endl;
    }

    return 0;
}
