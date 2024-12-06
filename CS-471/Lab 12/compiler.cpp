#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

vector<string> tac; // Stores the Three-Address Code (TAC) instructions

enum TokenType
{
    T_INT,
    T_ID,
    T_NUM,
    T_WHILE,
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
    T_LT, // < Operator
    T_GT, // > Operator
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
                else if (word == "while")
                    tokens.push_back(Token{T_WHILE, word, line});
                else
                    tokens.push_back(Token{T_ID, word, line});
                continue;
            }

            switch (current)
            {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "=", line});
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
            case '<':
                tokens.push_back(Token{T_LT, "<", line}); // Tokenize <
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", line}); // Tokenize >
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
        string name = tokens[pos].value;
        expect(T_ID);
        expect(T_ASSIGN);
        int tempVarIndex = parseExpression();
        expect(T_SEMICOLON);

        tac.push_back(name + " = t" + to_string(tempVarIndex));
        symbolTable.setSymbolValue(name, tempVarIndex);
    }

    void parseWhileLoop()
{
    expect(T_WHILE);            // Expect 'while'
    expect(T_LPAREN);           // Expect '('

    int startLabel = createLabel();    // Label for the start of the loop
    int trueLabel = createLabel();     // Label for the true condition (loop body)
    int endLabel = createLabel();      // Label for the end of the loop

    tac.push_back("L" + to_string(startLabel) + ":"); // Start of the loop

    // Parse the condition
    int conditionTemp = parseExpression();
    tac.push_back("if t" + to_string(conditionTemp) + " goto L" + to_string(trueLabel));
    tac.push_back("goto L" + to_string(endLabel));

    expect(T_RPAREN);           // Expect ')'

    tac.push_back("L" + to_string(trueLabel) + ":"); // Loop body label
    parseBlock();                                    // Parse the loop body
    tac.push_back("goto L" + to_string(startLabel)); // Jump back to the start of the loop

    tac.push_back("L" + to_string(endLabel) + ":"); // End of the loop
}



    void parseBlock()
    {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    int parseExpression()
{
    // Parse the first operand (number or identifier)
    int tempVarIndex = createTemporaryVariable();

    if (tokens[pos].type == T_NUM)
    {
        tac.push_back("t" + to_string(tempVarIndex) + " = " + tokens[pos].value);
        pos++; // Consume the number
    }
    else if (tokens[pos].type == T_ID)
    {
        tac.push_back("t" + to_string(tempVarIndex) + " = " + tokens[pos].value);
        pos++; // Consume the identifier
    }
    else
    {
        cout << "Unexpected token in expression: " << tokens[pos].value << endl;
        exit(1);
    }

    // Handle binary operators and their right-hand operands
    while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS ||
           tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
    {
        string op = tokens[pos].value; // Capture the operator
        pos++; // Consume the operator

        // Parse the right-hand operand
        int rightTempVarIndex = createTemporaryVariable();
        if (tokens[pos].type == T_NUM)
        {
            tac.push_back("t" + to_string(rightTempVarIndex) + " = " + tokens[pos].value);
            pos++; // Consume the number
        }
        else if (tokens[pos].type == T_ID)
        {
            tac.push_back("t" + to_string(rightTempVarIndex) + " = " + tokens[pos].value);
            pos++; // Consume the identifier
        }
        else
        {
            cout << "Syntax error: Expected number or identifier but found '"
                 << tokens[pos].value << "' on line " << tokens[pos].line << endl;
            exit(1);
        }

        // Generate TAC for the binary operation
        int newTempVarIndex = createTemporaryVariable();
        tac.push_back("t" + to_string(newTempVarIndex) + " = t" + to_string(tempVarIndex) +
                      " " + op + " t" + to_string(rightTempVarIndex));
        tempVarIndex = newTempVarIndex; // Update tempVarIndex with the result
    }

    return tempVarIndex;
}



    int createTemporaryVariable()
    {
        static int tempVarCounter = 0;
        return tempVarCounter++;
    }

    int createLabel()
    {
        static int labelCounter = 0;
        return labelCounter++;
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
        int x = 0;

        while (x < 5) {
            x = x + 1;
        }
    )";

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    SymbolTable symbolTable;
    Parser parser(tokens, symbolTable);
    parser.parseProgram();

    cout << "\nThree-Address Code (TAC):" << endl;
    for (const string &instruction : tac)
    {
        cout << instruction << endl;
    }

    return 0;
}
