#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <sstream>

using namespace std;

enum TokenType {
    T_INT, T_ID, T_NUM, T_IF, T_ELSE, T_RETURN,
    T_ASSIGN, T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,
    T_SEMICOLON, T_GT, T_EOF,
    T_FLOAT, T_DOUBLE, T_STRING, T_CHAR,
    T_FLOAT_LITERAL, T_STRING_LITERAL, T_WHILE, T_FOR
};

struct Token {
    TokenType type;
    string value;
    int line;
};

class Lexer {
private:
    string src;
    size_t pos;
    int line;

public:
    Lexer(const string &src) {
        this->src = src;
        this->pos = 0;
        this->line = 0;
    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < src.size()) {
            char current = src[pos];

            if (current == '\n') {
                line++;
                pos++;
                continue;
            }

            if (current == '/' && src[pos + 1] == '/') {
                pos += 2;
                while (pos < src.size() && src[pos] != '\n') pos++;
                continue;
            }

            if (isspace(current)) {
                pos++;
                continue;
            }

            if (isdigit(current)) {
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
                continue;
            }

            if (isalpha(current)) {
                string word = consumeWord();
                if (word == "int") tokens.push_back(Token{T_INT, word, line});
                else if (word == "if") tokens.push_back(Token{T_IF, word, line});
                else if (word == "else") tokens.push_back(Token{T_ELSE, word, line});
                else if (word == "float") tokens.push_back(Token{T_FLOAT, word, line});
                else if (word == "double") tokens.push_back(Token{T_DOUBLE, word, line});
                else if (word == "string") tokens.push_back(Token{T_STRING, word, line});
                else if (word == "char") tokens.push_back(Token{T_CHAR, word, line});
                else if (word == "return") tokens.push_back(Token{T_RETURN, word, line});
                else if (word == "while") tokens.push_back(Token{T_WHILE, word, line});
                else if (word == "for") tokens.push_back(Token{T_FOR, word, line});
                else tokens.push_back(Token{T_ID, word, line});
                continue;
            }

            switch (current) {
                case '=': tokens.push_back(Token{T_ASSIGN, "=", line}); break;
                case '+': tokens.push_back(Token{T_PLUS, "+", line}); break;
                case '-': tokens.push_back(Token{T_MINUS, "-", line}); break;
                case '*': tokens.push_back(Token{T_MUL, "*", line}); break;
                case '/': tokens.push_back(Token{T_DIV, "/", line}); break;
                case '(': tokens.push_back(Token{T_LPAREN, "(", line}); break;
                case ')': tokens.push_back(Token{T_RPAREN, ")", line}); break;
                case '{': tokens.push_back(Token{T_LBRACE, "{", line}); break;
                case '}': tokens.push_back(Token{T_RBRACE, "}", line}); break;
                case ';': tokens.push_back(Token{T_SEMICOLON, ";", line}); break;
                case '>': tokens.push_back(Token{T_GT, ">", line}); break;
                default:
                    cout << "Unexpected character '" << current << "' at line " << line << endl;
                    exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});
        return tokens;
    }

    string consumeNumber() {
        size_t start = pos;
        bool dotFlag = false;
        while (pos < src.size() && (isdigit(src[pos]) || (src[pos] == '.' && !dotFlag))) {
            if (src[pos] == '.') {
                dotFlag = true;
            }
            pos++;
        }
        return src.substr(start, pos - start);
    }

    string consumeWord() {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos])) pos++;
        return src.substr(start, pos - start);
    }
};

class ThreeAddressCodeGenerator {
private:
    vector<string> instructions;
    int tempCount;
    int labelCount;

    string newTemp() {
        return "t" + to_string(tempCount++);
    }

    string newLabel() {
        return "L" + to_string(labelCount++);
    }

public:
    ThreeAddressCodeGenerator() : tempCount(0), labelCount(0) {}

    void emit(const string &instruction) {
        instructions.push_back(instruction);
    }

    void printCode() const {
        for (const auto &inst : instructions) {
            cout << inst << endl;
        }
    }
   vector<string> getCode() const {
        return instructions;
    }
    string generateExpressionCode(const string &lhs, const string &op, const string &rhs) {
        string temp = newTemp();
        emit(temp + " = " + lhs + " " + op + " " + rhs);
        return temp;
    }

    string generateAssignmentCode(const string &var, const string &expr) {
        emit(var + " = " + expr);
        return var;
    }

    void generateWhileLoopCode(const string &condition, const vector<string> &body) {
        string startLabel = newLabel();
        string endLabel = newLabel();

        emit(startLabel + ":");
        emit("if False " + condition + " goto " + endLabel);

        for (const auto &inst : body) {
            emit(inst);
        }

        emit("goto " + startLabel);
        emit(endLabel + ":");
    }

    void generateForLoopCode(const string &init, const string &condition, const string &update, const vector<string> &body) {
        string startLabel = newLabel();
        string endLabel = newLabel();

        emit(init);

        emit(startLabel + ":");
        emit("if False " + condition + " goto " + endLabel);

        for (const auto &inst : body) {
            emit(inst);
        }

        emit(update);
        emit("goto " + startLabel);
        emit(endLabel + ":");
    }
};

class Parser {
public:
    Parser(const vector<Token> &tokens, ThreeAddressCodeGenerator &tac) : tokens(tokens), tac(tac), pos(0) {}

    void parseProgram() {
        while (tokens[pos].type != T_EOF) {
            parseStatement();
        }
    }

private:
    vector<Token> tokens;
    ThreeAddressCodeGenerator &tac;
    size_t pos;

    void parseStatement() {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_DOUBLE ||
            tokens[pos].type == T_STRING || tokens[pos].type == T_CHAR) {
            parseDeclaration();
        } else if (tokens[pos].type == T_ID) {
            parseAssignment();
        } else if (tokens[pos].type == T_IF) {
            parseIfStatement();
        } else if (tokens[pos].type == T_RETURN) {
            parseReturnStatement();
        } else if (tokens[pos].type == T_WHILE) {
            parseWhileStatement();
        } else if (tokens[pos].type == T_FOR) {
            parseForStatement();
        } else {
            pos++;
        }
    }

    void parseDeclaration() {
        pos++;
        string var = tokens[pos++].value;
        tac.emit("Declare " + var);
        if (tokens[pos].type == T_SEMICOLON) pos++;
    }

    void parseAssignment() {
        string var = tokens[pos++].value;
        pos++;
        string expr = parseExpression();
        tac.generateAssignmentCode(var, expr);
        if (tokens[pos].type == T_SEMICOLON) pos++;
    }

    string parseExpression() {
        string lhs = parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            string op = tokens[pos++].value;
            string rhs = parseTerm();
            lhs = tac.generateExpressionCode(lhs, op, rhs);
        }
        return lhs;
    }

    string parseTerm() {
        string lhs = parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
            string op = tokens[pos++].value;
            string rhs = parseFactor();
            lhs = tac.generateExpressionCode(lhs, op, rhs);
        }
        return lhs;
    }

    string parseFactor() {
        string value = tokens[pos].value;
        pos++;
        return value;
    }

    void parseIfStatement() {
        pos++;
        pos++;
        parseExpression();
        pos++;
        parseStatement();
    }

    void parseReturnStatement() {
        pos++;
        parseExpression();
        pos++;
    }

    void parseWhileStatement() {
        pos++;
        pos++;
        string condition = parseExpression();
        pos++;
        vector<string> body;
        parseLoopBody(body);
        tac.generateWhileLoopCode(condition, body);
    }

    void parseForStatement() {
        pos++;
        pos++;
        string init = parseAssignmentStatement();
        string condition = parseExpression();
        pos++;
        string update = parseAssignmentStatement();
        pos++;
        vector<string> body;
        parseLoopBody(body);
        tac.generateForLoopCode(init, condition, update, body);
    }

    string parseAssignmentStatement() {
        string var = tokens[pos++].value;
        pos++;
        string expr = parseExpression();
        tac.generateAssignmentCode(var, expr);
        if (tokens[pos].type == T_SEMICOLON) pos++;
        return var + " = " + expr;
    }

    void parseLoopBody(vector<string> &body) {
        pos++;
        while (tokens[pos].type != T_RBRACE) {
            body.push_back("...");  // Placeholder
            pos++;
        }
        pos++;
    }
};
class AssemblyCodeGenerator {
private:
    vector<string> assemblyInstructions;
    map<string, string> registerAllocation;
    int registerCount;

    string allocateRegister(const string &temp) {
        if (registerAllocation.find(temp) == registerAllocation.end()) {
            registerAllocation[temp] = "R" + to_string(registerCount++);
        }
        return registerAllocation[temp];
    }

public:
    AssemblyCodeGenerator() : registerCount(0) {}

    void generateAssembly(const vector<string> &threeAddressCode) {
        for (const auto &instr : threeAddressCode) {
            parseInstruction(instr);
        }
    }

    void parseInstruction(const string &instr) {
        stringstream ss(instr);
        string op;
        ss >> op;

        if (op == "Declare") {
            // Skip declaration for assembly
            return;
        }

        if (op == "ifFalse") {
            string condition, label;
            ss >> condition >> label >> label;
            assemblyInstructions.push_back("CMP " + condition + ", 0");
            assemblyInstructions.push_back("JE " + label);
        } else if (op == "goto") {
            string label;
            ss >> label;
            assemblyInstructions.push_back("JMP " + label);
        } else if (instr.find(" = ") != string::npos) {
            // Process assignment and arithmetic
            string dest, equals, arg1, oper, arg2;
            ss >> dest >> equals >> arg1;

            if (ss >> oper >> arg2) {  // Handling arithmetic operation
                string reg1 = allocateRegister(arg1);
                string reg2 = allocateRegister(arg2);
                string destReg = allocateRegister(dest);

                assemblyInstructions.push_back("MOV " + reg1 + ", " + arg1);
                if (oper == "+")
                    assemblyInstructions.push_back("ADD " + destReg + ", " + reg1 + ", " + reg2);
                else if (oper == "-")
                    assemblyInstructions.push_back("SUB " + destReg + ", " + reg1 + ", " + reg2);
                else if (oper == "*")
                    assemblyInstructions.push_back("MUL " + destReg + ", " + reg1 + ", " + reg2);
                else if (oper == "/")
                    assemblyInstructions.push_back("DIV " + destReg + ", " + reg1 + ", " + reg2);
            } else {  // Simple assignment
                string destReg = allocateRegister(dest);
                assemblyInstructions.push_back("MOV " + destReg + ", " + arg1);
            }
        } else if (op.find("return") != string::npos) {
            string returnValue;
            ss >> returnValue;
            string returnReg = allocateRegister(returnValue);
            assemblyInstructions.push_back("MOV R0, " + returnReg); // Return value in R0
        }
    }

    void printAssembly() const {
        for (const auto &instr : assemblyInstructions) {
            cout << instr << endl;
        }
    }
};
int main() {
    string input = R"(
        int a;
        a = 5;
        int b;
        b = a + 10;
        if (b > 10) {
            return b;
        }
        for(x = 0; x > 2; x++){
            a = x + 1;
        }
    )";

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    ThreeAddressCodeGenerator tac;
    Parser parser(tokens, tac);
    parser.parseProgram();

    cout << "Three-Address Code:" << endl;
    tac.printCode();



    // Step 3: Assembly Code Generation
    AssemblyCodeGenerator asmGen;
    asmGen.generateAssembly(tac.getCode()); // Assuming `getCode()` returns the TAC as a vector of strings

    // Print the generated Assembly Code
    cout << "\nAssembly Code:" << endl;
    asmGen.printAssembly();

    return 0;
}
