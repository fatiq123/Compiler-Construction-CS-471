#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <fstream>

using namespace std;

enum TokenType
{
    T_INT,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_RETURN,
    T_WHILE,
    T_FOR,
    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_SEMICOLON,
    T_GT,
    T_LT,
    T_EQ,
    T_NEQ,
    T_GTE,
    T_LTE,
    T_EOF,
};

struct Token
{
    TokenType type;
    string value;
    int line;
};

struct Symbol
{
    int value;
};

class Lexer
{
private:
    string src;
    size_t pos;
    int line;

public:
    Lexer(const string &src) : src(src), pos(0), line(1) {}

    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];

            if (isspace(current))
            {
                if (current == '\n')
                    line++;
                pos++;
                continue;
            }

            if (current == '/' && pos + 1 < src.size())
            {
                if (src[pos + 1] == '/')
                {
                    skipSingleLineComment();
                    continue;
                }
                else if (src[pos + 1] == '*')
                {
                    skipMultiLineComment();
                    continue;
                }
            }

            if (isdigit(current))
            {
                size_t start = pos;
                while (pos < src.size() && isdigit(src[pos]))
                    pos++;
                tokens.push_back(Token{T_NUM, src.substr(start, pos - start), line});
                continue;
            }

            if (isalpha(current))
            {
                size_t start = pos;
                while (pos < src.size() && isalnum(src[pos]))
                    pos++;
                string word = src.substr(start, pos - start);
                if (word == "int")
                    tokens.push_back(Token{T_INT, word, line});
                else if (word == "if")
                    tokens.push_back(Token{T_IF, word, line});
                else if (word == "else")
                    tokens.push_back(Token{T_ELSE, word, line});
                else if (word == "return")
                    tokens.push_back(Token{T_RETURN, word, line});
                else if (word == "while")
                    tokens.push_back(Token{T_WHILE, word, line});
                else if (word == "for")
                    tokens.push_back(Token{T_FOR, word, line});
                else
                    tokens.push_back(Token{T_ID, word, line});
                continue;
            }

            switch (current)
            {
            case '=':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_EQ, "==", line});
                    pos++;
                }
                else
                    tokens.push_back(Token{T_ASSIGN, "=", line});
                break;
            case '!':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_NEQ, "!=", line});
                    pos++;
                }
                break;
            case '<':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_LTE, "<=", line});
                    pos++;
                }
                else
                    tokens.push_back(Token{T_LT, "<", line});
                break;
            case '>':
                if (pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_GTE, ">=", line});
                    pos++;
                }
                else
                    tokens.push_back(Token{T_GT, ">", line});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", line});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", line});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", line});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", line});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", line});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", line});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", line});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", line});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", line});
                break;
            default:
                cout << "Unexpected character: " << current << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});
        return tokens;
    }

private:
    void skipSingleLineComment()
    {
        while (pos < src.size() && src[pos] != '\n')
        {
            pos++;
        }
    }

    void skipMultiLineComment()
    {
        pos += 2;
        while (pos < src.size())
        {
            if (src[pos] == '*' && pos + 1 < src.size() && src[pos + 1] == '/')
            {
                pos += 2;
                break;
            }
            pos++;
        }
    }
};

class SymbolTable
{
private:
    map<string, Symbol> symbols;

public:
    void addSymbol(const string &name, int value)
    {
        symbols[name] = {value};
    }

    bool getSymbolValue(const string &name, int &value)
    {
        if (symbols.find(name) != symbols.end())
        {
            value = symbols[name].value;
            return true;
        }
        return false;
    }

    void setSymbolValue(const string &name, int value)
    {
        if (symbols.find(name) != symbols.end())
        {
            symbols[name].value = value;
        }
        else
        {
            addSymbol(name, value);
        }
    }
};

class Parser
{
private:
    vector<Token> tokens;
    size_t pos;
    SymbolTable &symbolTable;

public:
    Parser(const vector<Token> &tokens, SymbolTable &symbolTable) : tokens(tokens), pos(0), symbolTable(symbolTable) {}

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        cout << "Execution completed successfully!" << endl;
    }

private:
    void parseStatement()
    {
        if (tokens[pos].type == T_INT)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_WHILE)
        {
            parseWhileLoop();
        }
        else if (tokens[pos].type == T_FOR)
        {
            parseForLoop();
        }
        else
        {
            cout << "Syntax error at: " << tokens[pos].value << endl;
            exit(1);
        }
    }

    void parseDeclaration()
    {
        expect(T_INT);
        string name = tokens[pos].value;
        expect(T_ID);
        expect(T_ASSIGN);
        int value = parseExpression();
        expect(T_SEMICOLON);
        symbolTable.addSymbol(name, value);
    }

    void parseAssignment()
    {
        string name = tokens[pos].value;         // Get the variable name
        expect(T_ID);                            // Expect an identifier
        expect(T_ASSIGN);                        // Expect '='
        int value = parseExpression();           // Parse and evaluate the expression
        expect(T_SEMICOLON);                     // Expect ';'
        symbolTable.setSymbolValue(name, value); // Update the symbol table
    }

    void parseWhileLoop()
    {
        expect(T_WHILE);  // Expect 'while'
        expect(T_LPAREN); // Expect '('

        size_t conditionStart = pos;                  // Save the position of the condition
        bool condition = parseRelationalExpression(); // Parse and evaluate the condition
        expect(T_RPAREN);                             // Expect ')'

        size_t loopBodyStart = pos; // Save the position of the loop body

        while (condition) // Execute the loop while the condition is true
        {
            pos = loopBodyStart;                     // Reset to the start of the loop body
            parseBlock();                            // Execute the loop body
            pos = conditionStart;                    // Reset to the start of the condition
            condition = parseRelationalExpression(); // Re-evaluate the condition
        }

        // After loop ends, skip over the loop body
        pos = loopBodyStart; // Reset to the loop body
        parseBlock();        // Skip the loop body (to exit the loop)
    }

    void parseForLoop()
    {
        expect(T_FOR);    // Expect 'for'
        expect(T_LPAREN); // Expect '('

        parseDeclaration();                           // Parse the initialisation statement (e.g., int i = 0)
        bool condition = parseRelationalExpression(); // Parse the condition (e.g., i < 10)
        expect(T_SEMICOLON);                          // Expect ';'

        size_t incrementStart = pos; // Remember position of the increment statement
        parseStatement();            // Parse the increment statement (e.g., i = i + 1)
        expect(T_RPAREN);            // Expect ')'

        while (condition) // Execute the loop while the condition is true
        {
            parseBlock(); // Parse the loop body

            // Reevaluate the increment statement and condition
            pos = incrementStart;                    // Reset to increment statement
            parseStatement();                        // Execute the increment
            condition = parseRelationalExpression(); // Reevaluate the condition
        }
    }

    void parseBlock()
    {
        expect(T_LBRACE); // Expect '{'

        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseStatement(); // Parse and execute each statement in the block
        }

        expect(T_RBRACE); // Expect '}'
    }

    bool parseRelationalExpression()
    {
        int left = parseExpression(); // Parse the left operand

        // Ensure the next token is a relational operator
        if (tokens[pos].type == T_LT || tokens[pos].type == T_GT || tokens[pos].type == T_EQ ||
            tokens[pos].type == T_NEQ || tokens[pos].type == T_LTE || tokens[pos].type == T_GTE)
        {
            TokenType op = tokens[pos].type;
            pos++; // Consume the relational operator

            int right = parseExpression(); // Parse the right operand

            // Evaluate the relational expression
            switch (op)
            {
            case T_LT:
                return left < right;
            case T_GT:
                return left > right;
            case T_EQ:
                return left == right;
            case T_NEQ:
                return left != right;
            case T_LTE:
                return left <= right;
            case T_GTE:
                return left >= right;
            default:
                cout << "Invalid relational operator!" << endl;
                exit(1);
            }
        }
        else
        {
            cout << "Syntax error: Expected relational operator but found '"
                 << tokens[pos].value << "' on line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    int parseExpression()
    {
        // Parse the first operand
        int left = 0;

        if (tokens[pos].type == T_NUM)
        {
            left = stoi(tokens[pos].value); // Convert number token to an integer
            pos++;                          // Move to the next token
        }
        else if (tokens[pos].type == T_ID)
        {
            string name = tokens[pos].value;
            pos++;                                       // Move to the next token
            if (!symbolTable.getSymbolValue(name, left)) // Get the variable value
            {
                cout << "Undefined variable: " << name << endl;
                exit(1);
            }
        }
        else
        {
            cout << "Unexpected token in expression: " << tokens[pos].value << endl;
            exit(1);
        }

        // Parse binary operators and their operands
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS ||
               tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            TokenType op = tokens[pos].type; // Capture the operator
            pos++;                           // Consume the operator

            // Parse the right operand
            int right = 0;
            if (tokens[pos].type == T_NUM)
            {
                right = stoi(tokens[pos].value);
                pos++;
            }
            else if (tokens[pos].type == T_ID)
            {
                string name = tokens[pos].value;
                pos++;
                if (!symbolTable.getSymbolValue(name, right))
                {
                    cout << "Undefined variable: " << name << endl;
                    exit(1);
                }
            }
            else
            {
                cout << "Syntax error: Expected number or identifier but found '"
                     << tokens[pos].value << "' on line " << tokens[pos].line << endl;
                exit(1);
            }

            // Perform the operation
            switch (op)
            {
            case T_PLUS:
                left += right;
                break;
            case T_MINUS:
                left -= right;
                break;
            case T_MUL:
                left *= right;
                break;
            case T_DIV:
                if (right == 0)
                {
                    cout << "Error: Division by zero on line " << tokens[pos].line << endl;
                    exit(1);
                }
                left /= right;
                break;
            default:
                cout << "Invalid operator in expression!" << endl;
                exit(1);
            }
        }

        return left; // Return the result of the expression
    }

    void expect(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            pos++;
        }
        else
        {
            cout << "Expected token type but found: " << tokens[pos].value << endl;
            exit(1);
        }
    }
};

int main()
{
    string input = R"(
    int x = 0;          // Declare and initialise variable

    while (x < 5) {     // While loop condition
        x = x + 1;      // Increment x
    }
)";

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    SymbolTable symbolTable;
    Parser parser(tokens, symbolTable);
    parser.parseProgram();

    return 0;
}
