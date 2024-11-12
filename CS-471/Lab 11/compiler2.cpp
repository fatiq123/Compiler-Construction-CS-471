#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

enum TokenType
{
    T_INT,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_RETURN,
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
    T_EOF,
};

struct Token
{
    TokenType type;
    string value;
};

class Lexer
{
private:
    string src;
    size_t pos;

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

    string consumeNumber()
    {
        size_t start = pos;
        while (pos < src.size() && isdigit(src[pos]))
            pos++;
        return src.substr(start, pos - start);
    }

    string consumeWord()
    {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos]))
            pos++;
        return src.substr(start, pos - start);
    }

public:
    Lexer(const string &src) : src(src), pos(0) {}

    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];

            if (isspace(current))
            {
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
                tokens.push_back(Token{T_NUM, consumeNumber()});
                continue;
            }

            if (isalpha(current))
            {
                string word = consumeWord();
                if (word == "int")
                    tokens.push_back(Token{T_INT, word});
                else if (word == "if")
                    tokens.push_back(Token{T_IF, word});
                else if (word == "else")
                    tokens.push_back(Token{T_ELSE, word});
                else if (word == "return")
                    tokens.push_back(Token{T_RETURN, word});
                else
                    tokens.push_back(Token{T_ID, word});
                continue;
            }

            switch (current)
            {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "="});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+"});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-"});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*"});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/"});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "("});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")"});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{"});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}"});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";"});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">"});
                break;
            default:
                cout << "Unexpected character: " << current << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, ""}); // Add EOF token at the end
        return tokens;
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
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else
        {
            cout << "Syntax error: unexpected token " << tokens[pos].value << endl;
            exit(1);
        }
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

    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression()
    { // Simplified for demonstration
        // Handle simple expressions for now
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID)
        {
            pos++; // Consume number or identifier
        }
        else
        {
            cout << "Syntax error: expected expression but found " << tokens[pos].value << endl;
            exit(1);
        }

        // Add more logic for handling operators and complex expressions as needed
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS ||
               tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            pos++; // Consume operator
            // Expecting another number or identifier after operator
            if (tokens[pos].type != T_NUM && tokens[pos].type != T_ID)
            {
                cout << "Syntax error: expected expression after operator but found " << tokens[pos].value << endl;
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
            cout << "Syntax error: expected token type but found " << tokens[pos].value << endl;
            exit(1);
        }
    }
};

int main()
{
    // Sample input with comments
    string input = R"(
        // This is a single line comment
        int x; /* This is a multi-line comment
                 that should be skipped */
        x = 5; // Assigning value to x
    )";

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    // Output tokens to verify if comments were skipped
    for (const Token &token : tokens)
    {
        cout << "Token: " << token.value << " Type: " << token.type << endl;
    }

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}