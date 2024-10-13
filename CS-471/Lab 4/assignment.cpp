#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>

using namespace std;

// Enum for token types
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    DELIMITER,
    STRING,
    EOF_TOKEN
};

// Token structure
struct Token {
    TokenType type;
    string value;

    Token(TokenType t, string v) : type(t), value(v) {}
};

// Tokenizer class
class Tokenizer {
private:
    // Keywords, operators, and delimiters
    unordered_set<string> keywords = {"if", "else", "while", "return", "function"};
    unordered_set<string> operators = {"+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">="};
    unordered_set<char> delimiters = {';', '(', ')', '{', '}', ','};

    vector<Token> tokens;
    
    // Regular expressions for matching different types
    regex numberRegex = regex(R"(\d+(\.\d*)?)");
    regex identifierRegex = regex(R"([A-Za-z_]\w*)");
    regex operatorRegex = regex(R"([+\-*/=!<>]+)");
    regex stringRegex = regex(R"("[^"]*")");
    
    // Helper function to check if a string is a keyword
    bool isKeyword(const string& str) {
        return keywords.find(str) != keywords.end();
    }

    // Helper function to check if a string is an operator
    bool isOperator(const string& str) {
        return operators.find(str) != operators.end();
    }

    // Helper function to check if a char is a delimiter
    bool isDelimiter(char ch) {
        return delimiters.find(ch) != delimiters.end();
    }

public:
    // Tokenizer function
    vector<Token> tokenize(const string& code) {
        size_t pos = 0;
        while (pos < code.size()) {
            // Skip whitespace
            if (isspace(code[pos])) {
                pos++;
                continue;
            }

            // Match numbers
            if (isdigit(code[pos])) {
                smatch match;
                string remainingCode = code.substr(pos);
                if (regex_search(remainingCode, match, numberRegex)) {
                    tokens.emplace_back(TokenType::NUMBER, match.str());
                    pos += match.length();
                    continue;
                }
            }

            // Match identifiers and keywords
            if (isalpha(code[pos]) || code[pos] == '_') {
                smatch match;
                string remainingCode = code.substr(pos);
                if (regex_search(remainingCode, match, identifierRegex)) {
                    string value = match.str();
                    if (isKeyword(value)) {
                        tokens.emplace_back(TokenType::KEYWORD, value);
                    } else {
                        tokens.emplace_back(TokenType::IDENTIFIER, value);
                    }
                    pos += match.length();
                    continue;
                }
            }

            // Match operators
            smatch match;
            string remainingCode = code.substr(pos);
            if (regex_search(remainingCode, match, operatorRegex)) {
                string value = match.str();
                if (isOperator(value)) {
                    tokens.emplace_back(TokenType::OPERATOR, value);
                    pos += match.length();
                    continue;
                }
            }

            // Match delimiters
            if (isDelimiter(code[pos])) {
                tokens.emplace_back(TokenType::DELIMITER, string(1, code[pos]));
                pos++;
                continue;
            }

            // Match string literals
            if (code[pos] == '"') {
                smatch match;
                string remainingCode = code.substr(pos);
                if (regex_search(remainingCode, match, stringRegex)) {
                    tokens.emplace_back(TokenType::STRING, match.str());
                    pos += match.length();
                    continue;
                }
            }

            // Unexpected character
            cout << "Unexpected character: " << code[pos] << endl;
            pos++;
        }

        // Add EOF token
        tokens.emplace_back(TokenType::EOF_TOKEN, "");
        return tokens;
    }

    // Helper function to print tokens
    void printTokens() {
        for (const auto& token : tokens) {
            switch (token.type) {
                case TokenType::KEYWORD:
                    cout << "Keyword: " << token.value << endl;
                    break;
                case TokenType::IDENTIFIER:
                    cout << "Identifier: " << token.value << endl;
                    break;
                case TokenType::NUMBER:
                    cout << "Number: " << token.value << endl;
                    break;
                case TokenType::OPERATOR:
                    cout << "Operator: " << token.value << endl;
                    break;
                case TokenType::DELIMITER:
                    cout << "Delimiter: " << token.value << endl;
                    break;
                case TokenType::STRING:
                    cout << "String: " << token.value << endl;
                    break;
                case TokenType::EOF_TOKEN:
                    cout << "End of File" << endl;
                    break;
            }
        }
    }
};

// Main function to demonstrate the tokenizer
int main() {
    string code = R"(if (x == 10) { return x + 2; })";

    Tokenizer tokenizer;
    vector<Token> tokens = tokenizer.tokenize(code);

    // Print the tokens
    tokenizer.printTokens();

    return 0;
}
