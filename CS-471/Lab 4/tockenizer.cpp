#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_set>

using namespace std;

// Define keywords and symbols
unordered_set<string> keywords = {"int", "float", "return", "if", "else", "switch", ""};
unordered_set<char> symbols = {'(', ')', '{', '}', ';', ','};

void tokenize(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    string token;
    char ch;
    while (file.get(ch)) {
        // Ignore spaces
        if (isspace(ch)) continue;

        // Handle symbols
        if (symbols.count(ch)) {
            if (!token.empty()) {
                // If there is a token before the symbol, process it
                if (keywords.count(token)) {
                    cout << "Keyword: " << token << endl;
                } else {
                    cout << "Identifier: " << token << endl;
                }
                token.clear();
            }
            cout << "Symbol: " << ch << endl;
            continue;
        }

        // Handle alphanumeric tokens (keywords/identifiers)
        if (isalnum(ch)) {
            token += ch;
        } else {
            if (!token.empty()) {
                // If a non-alphanumeric character is found after a token, print it
                if (keywords.count(token)) {
                    cout << "Keyword: " << token << endl;
                } else {
                    cout << "Identifier: " << token << endl;
                }
                token.clear();
            }
            cout << "Operator: " << ch << endl;
        }
    }

    // If there's a leftover token at the end of the file
    if (!token.empty()) {
        if (keywords.count(token)) {
            cout << "Keyword: " << token << endl;
        } else {
            cout << "Identifier: " << token << endl;
        }
    }

    file.close();
}

int main() {
    string filename = "file.txt";  // Your file name here
    tokenize(filename);
    return 0;
}
