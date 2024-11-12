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
    T_EOF,
};

struct Token
{
    TokenType type;
    string value;
    int line; // Add line number to the token
};

struct Symbol
{
    string type; // Variable type (e.g., int)
    int line;    // Line number where it was declared
    int scope;   // Scope level
};

class Lexer
{
private:
    string src;
    size_t pos;
    int line; // Current line number

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
                    line++; // Increment line on newline
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
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", line});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", line});
                break;
            case '<':
                tokens.push_back(Token{T_LT, "<", line});
                break;
            default:
                cout << "Unexpected character: " << current << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line}); // Add EOF token at the end
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
        pos += 2; // Skip initial /*
        while (pos < src.size())
        {
            if (src[pos] == '*' && pos + 1 < src.size() && src[pos + 1] == '/')
            {
                pos += 2; // Skip closing */
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
    int currentScope = 0;

public:                         
    void enterScope() { 
        currentScope++;
    }

    void exitScope()
    {
        // Remove all symbols with the current scope level
        for (auto it = symbols.begin(); it != symbols.end();)
        {
            if (it->second.scope == currentScope)
                it = symbols.erase(it);
            else
                ++it;
        }
        currentScope--;
    }

    bool addSymbol(const string &name, const string &type, int line)
    {
        if (symbols.find(name) == symbols.end() || symbols[name].scope != currentScope)
        {
            symbols[name] = {type, line, currentScope};
            return true;
        }
        return false; // Symbol already exists in the current scope
    }

    bool symbolExists(const string &name)
    {
        return symbols.find(name) != symbols.end() && symbols[name].scope <= currentScope;
    }

    void printSymbols()
    {
        cout << "\nSymbol Table:\n";
        cout << "Identifier\tType\tScope\tLine\n";
        for (const auto &entry : symbols)
        {
            cout << entry.first << "\t\t" << entry.second.type << "\t" << entry.second.scope << "\t" << entry.second.line << "\n";
        }
    }
};


class Parser
{
public:
    Parser(const vector<Token> &tokens) : tokens(tokens), pos(0) {}

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        { // Continue until EOF
            parseStatement();
        }
        cout << "Parsing completed successfully! No Syntax Error" << endl;
    }

private:
    vector<Token> tokens;
    size_t pos;

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
        else if (tokens[pos].type == T_IF)
        {
            parseIfStatement();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[pos].type == T_WHILE)
        {
            parseWhileStatement();
        }
        else if (tokens[pos].type == T_FOR)
        {
            parseForStatement();
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else
        {
            cout << "Syntax error: unexpected token '" << tokens[pos].value
                 << "' on line " << tokens[pos].line << endl;
            exit(1);
        }
    }

void parseFunctionDeclaration()
{
    expect(T_INT);         // Expect 'int' keyword
    expect(T_ID);          // Expect function name (e.g., main)
    expect(T_LPAREN);      // Expect '('
    expect(T_RPAREN);      // Expect ')'
    parseBlock();          // Parse function body
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

    void parseDeclaration()
    {
        expect(T_INT);
        expect(T_ID);
        expect(T_SEMICOLON);
    }

    void parseAssignment()
    {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE)
        {
            expect(T_ELSE);
            parseStatement();
        }
    }

    void parseWhileStatement()
    {
        expect(T_WHILE);
        expect(T_LPAREN);
        parseExpression(); // Condition
        expect(T_RPAREN);
        parseStatement(); // Body
    }

    void parseForStatement()
    {
        expect(T_FOR);
        expect(T_LPAREN);
        // For loop initialization
        parseAssignment(); // Example: int j = 0
        expect(T_SEMICOLON);
        // For loop condition
        parseExpression(); // Example: j < 5
        expect(T_SEMICOLON);
        // For loop increment
        expect(T_ID); // Example: j
        expect(T_ASSIGN);
        expect(T_ID); // Example: j
        expect(T_PLUS);
        expect(T_NUM); // Example: 1
        expect(T_RPAREN);
        parseStatement(); // Body
    }

    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression()
    { // Simplified for demonstration...
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID)
        {
            pos++; // Consume number or identifier
        }
        else
        {
            cout << "Syntax error: expected expression but found '" << tokens[pos].value
                 << "' on line " << tokens[pos].line << endl;
            exit(1);
        }

        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS ||
               tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            pos++; // Consume operator
            if (tokens[pos].type != T_NUM && tokens[pos].type != T_ID)
            {
                cout << "Syntax error: expected expression after operator but found '"
                     << tokens[pos].value << "' on line " << tokens[pos].line << endl;
                exit(1);
            }
            pos++; // Consume next number or identifier
        }
    }

    void expect(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            pos++;
        }
        else
        {
            cout << "Syntax error: expected token type but found '" << tokens[pos].value
                 << "' on line " << tokens[pos].line << endl;
            exit(1);
        }
    }
};

int main()
{
    string input = R"(
        int main() {
            int i = 0;  // Loop counter

            while(i < 10) {   // While loop example
                i = i + 1;   // Increment counter
            }

            for(int j = 0; j < 5; j = j + 1) {   // For loop example
                j = j * 2;   // Some operation inside loop
            }
            
            return i;   // Return statement example
        }
    )";

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    // Output tokens with their values and lines for verification.
    for (const Token &token : tokens)
    {
        cout << "Token: '" << token.value << "' on Line: " << token.line << endl;
    }

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}