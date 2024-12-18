#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <map>
#include <sstream>
#include <stdexcept>

using namespace std;

enum TokenType
{
    T_INT,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_RETURN,
    T_FOR,
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
    T_GT,
    T_EOF
};

struct Token
{
    TokenType type;
    string value;
    int lineNumber;
};

struct Point
{
    int x;
    int y;
};

class Lexer
{
private:
    string src;
    size_t pos;
    int lineNumber;

public:
    Lexer(const string &src) : src(src), pos(0), lineNumber(1) {}

    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];

            if (current == '\n')
            {
                lineNumber++;
                pos++;
                continue;
            }
            if (isspace(current))
            {
                pos++;
                continue;
            }
            if (isdigit(current))
            {
                tokens.push_back(Token{T_NUM, consumeNumber(), lineNumber});
                continue;
            }
            if (isalpha(current))
            {
                string word = consumeWord();
                TokenType type = T_ID;

                if (word == "int")
                    type = T_INT;
                else if (word == "if")
                    type = T_IF;
                else if (word == "else")
                    type = T_ELSE;
                else if (word == "for")
                    type = T_FOR;
                else if (word == "while")
                    type = T_WHILE;
                else if (word == "return")
                    type = T_RETURN;

                tokens.push_back(Token{type, word, lineNumber});
                continue;
            }

            switch (current)
            {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "=", lineNumber});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", lineNumber});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", lineNumber});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", lineNumber});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", lineNumber});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", lineNumber});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", lineNumber});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", lineNumber});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", lineNumber});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", lineNumber});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", lineNumber});
                break;
            default:
                cout << "Unexpected character: " << current << " at line " << lineNumber << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", lineNumber});
        return tokens;
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
};

/*
    SymbolTable class:

    The SymbolTable class manages a collectin of variable names and their associated data types.
    It serves as a fundamental part of a compiler or interpreter, ensuring that variables are properly declared
    and accessed with their correct types during semantic analysis.


    Member Functions:
    1. declareVariable(const string &name, const string &type):
       - Purpose: Declares a new variable with a specified name and type.
       - This function checks if the variable is already declared. If it is, a runtime error is thrown indicating
         that the variable has already been declared.
       - If the variable is not already declared, it adds the variable's name as a key and its type as the value
         to the symbolTable map.
       - It will throw a runtime error if the variable already exists in the symbol table.
       - Example usage: Declare a new variable x with type int.

       if (symbolTable.find(name) != symbolTable.end()) {
           throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
       }
       symbolTable[name] = type;

    2. getVariableType(const string &name):
       - Purpose: Returns the type of a variable given its name.
       - This function checks if the variable exists in the symbolTable. If the variable is not found, it throws
         a runtime error indicating that the variable has not been declared yet.
       - If the variable is declared, it retrieves and returns its associated type from the symbol table.
       - Throws a runtime error if the variable is not found.
       - Example usage: Get the type of variable x.

       if (symbolTable.find(name) == symbolTable.end()) {
           throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
       }
       return symbolTable[name];

    3. isDeclared(const string &name) const:
       - Purpose: Checks whether a variable has been declared.
       - This function returns a boolean value indicating whether the variable exists in the symbolTable.
       - It is a quick way to check the existence of a variable without retrieving its type.
       - Example usage: Check if variable x is declared.

       return symbolTable.find(name) != symbolTable.end();

    Private Data Members:

    - map<string, string> symbolTable:
      - A map that stores variable names (strings) as keys and their associated types (strings) as values.
      - This map allows efficient lookups to check if a variable is declared and to retrieve its type.

    Usage in a Compiler or Interpreter:
    - The SymbolTable is crucial for ensuring that variables are used consistently and correctly in a program.
    - When a variable is declared, it is added to the symbol table, and its type is stored.
    - When a variable is referenced, the symbol table is consulted to ensure that it has been declared and to retrieve its type.
    - The symbol table helps detect semantic errors such as undeclared variables or redeclared variables.
*/
class SymbolTable
{
public:
    void declareVariable(const string &name, const string &type)
    {
        if (symbolTable.find(name) != symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
        }
        symbolTable[name] = type;
    }

    string getVariableType(const string &name)
    {
        if (symbolTable.find(name) == symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
        }
        return symbolTable[name];
    }

    bool isDeclared(const string &name) const
    {
        return symbolTable.find(name) != symbolTable.end();
    }

private:
    map<string, string> symbolTable;
};

class IntermediateCodeGnerator
{
public:
    vector<string> instructions;
    int tempCount = 0;

    string newTemp()
    {
        return "t" + to_string(tempCount++);
    }

    void addInstruction(const string &instr)
    {
        instructions.push_back(instr);
    }

    void printInstructions()
    {
        for (const auto &instr : instructions)
        {
            cout << instr << endl;
        }
    }
};

class Parser
{
public:
    // Constructor
    Parser(const vector<Token> &tokens, SymbolTable &symTable, IntermediateCodeGnerator &icg)
        : tokens(tokens), pos(0), symTable(symTable), icg(icg) {}
    // here the private member of this class are being initalized with the arguments passed to this constructor

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
    }

private:
    vector<Token> tokens;
    size_t pos;
    SymbolTable &symTable;
    IntermediateCodeGnerator &icg;

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
        else if (tokens[pos].type == T_FOR)
        { // Added for-loop handling
            parseForLoop();
        }
        else if (tokens[pos].type == T_WHILE)
        { // Added while-loop handling
            parseWhileLoop();
        }
        else
        {
            cout << "Syntax error: unexpected token '" << tokens[pos].value
                 << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
    }
    void parseForLoop()
    {
        expect(T_FOR);    // Consume the 'for' keyword.
        expect(T_LPAREN); // Expect and consume '('.

        // Step 1: Initialization (e.g., int i = 0;)
        parseAssignment(); // This assumes initialization is a regular assignment statement.

        string loopStartLabel = "L" + to_string(icg.tempCount++); // Start of the loop.
        icg.addInstruction(loopStartLabel + ":");

        // Step 2: Condition (e.g., i < 10;)
        string condition = parseExpression();   // Parse the loop condition.
        string conditionResult = icg.newTemp(); // Temporary variable for condition result.
        icg.addInstruction(conditionResult + " = " + condition);

        string loopEndLabel = "L" + to_string(icg.tempCount++); // Label for exiting the loop.
        icg.addInstruction("if " + conditionResult + " == 0 goto " + loopEndLabel);

        expect(T_SEMICOLON); // Expect and consume ';'.

        // Step 3: Update expression (e.g., i = i + 1)
        size_t updatePos = pos; // Save position to process the update later.
        while (tokens[pos].type != T_RPAREN)
            pos++;        // Skip to the end of the update part.
        expect(T_RPAREN); // Consume ')'.

        // Step 4: Loop body
        parseStatement(); // Parse the loop body.

        // Step 5: Generate update expression and loop back to condition
        string loopUpdateLabel = "L" + to_string(icg.tempCount++); // Label for update.
        icg.addInstruction(loopUpdateLabel + ":");
        pos = updatePos;   // Reset position to the update expression.
        parseAssignment(); // Parse the update statement.

        icg.addInstruction("goto " + loopStartLabel); // Jump back to the loop start.
        icg.addInstruction(loopEndLabel + ":");       // Label for loop exit.
    }
    void parseWhileLoop()
    {
        expect(T_WHILE);  // Consume the 'while' keyword.
        expect(T_LPAREN); // Expect and consume '('.

        // Step 1: Generate label for the start of the loop
        string loopStartLabel = "L" + to_string(icg.tempCount++); // Start of the loop.
        icg.addInstruction(loopStartLabel + ":");

        // Step 2: Parse the condition
        string condition = parseExpression();   // Parse the loop condition.
        string conditionResult = icg.newTemp(); // Temporary variable for condition result.
        icg.addInstruction(conditionResult + " = " + condition);

        // Step 3: Generate label for loop exit
        string loopEndLabel = "L" + to_string(icg.tempCount++); // Label for exiting the loop.
        icg.addInstruction("if " + conditionResult + " == 0 goto " + loopEndLabel);

        expect(T_RPAREN); // Expect and consume ')'.

        // Step 4: Parse the loop body
        parseStatement(); // Parse the loop body.

        // Step 5: Jump back to the start of the loop
        icg.addInstruction("goto " + loopStartLabel);

        // Step 6: Generate the loop exit label
        icg.addInstruction(loopEndLabel + ":");
    }

    /*
     parseDeclaration handles the parsing of variable declarations.
     It expects the token type to be T_INT (for declaring an integer type variable),
     followed by an identifier (variable name), and a semicolon to terminate the statement.
     It also registers the declared variable in the symbol table with type "int".
     Example:
     int x;   // This will be parsed and the symbol table will store x with type "int".
    */
    void parseDeclaration()
    {
        expect(T_INT);                               // Expect and consume the int keyword.
        string varName = expectAndReturnValue(T_ID); // Expect and return the variable name (identifier).
        symTable.declareVariable(varName, "int");    // Register the variable in the symbol table with type "int".
        expect(T_SEMICOLON);                         // Expect the semicolon to end the statement.
    }

    /*
     parseAssignment handles the parsing of variable assignments.
     It expects an identifier (variable name), an assignment operator =, followed by an expression,
     and a semicolon at the end of the statement.
     It checks if the variable is declared in the symbol table, parses the expression, and generates intermediate code
     for the assignment.
     Example:
     x = 10;   -->  This will be parsed, checking if x is declared, then generating intermediate code like x = 10.
    */
    void parseAssignment()
    {
        string varName = expectAndReturnValue(T_ID);
        symTable.getVariableType(varName); // Ensure the variable is declared in the symbol table.
        expect(T_ASSIGN);
        string expr = parseExpression();
        icg.addInstruction(varName + " = " + expr); // Generate intermediate code for the assignment.
        expect(T_SEMICOLON);
    }
    /*
         parseIfStatement handles the parsing of if statements.
         It expects the keyword if, followed by an expression in parentheses that serves as the condition.
         If the condition evaluates to true, it executes the statement inside the block. If an else part is present,
         it executes the corresponding statement after the else keyword.
         Intermediate code for the if statement is generated, including labels for conditional jumps.
         Example:
         if(5 > 3) { x = 20; }  --> This will generate intermediate code for the condition check and jump instructions.
    */
    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);                // Expect and consume the opening parenthesis for the condition.
        string cond = parseExpression(); // Parse the condition expression inside the parentheses.
        expect(T_RPAREN);

        string temp = icg.newTemp();             // Generate a new temporary variable for the condition result.
        icg.addInstruction(temp + " = " + cond); // Generate intermediate code for storing the condition result.

        icg.addInstruction("if " + temp + " goto L1"); // Jump to label L1 if condition is true.
        icg.addInstruction("goto L2");                 // Otherwise, jump to label L2.
        icg.addInstruction("L1:");                     // Otherwise, jump to label L2.

        parseStatement();

        if (tokens[pos].type == T_ELSE)
        { // If an else part exists, handle it.
            icg.addInstruction("goto L3");
            icg.addInstruction("L2:");
            expect(T_ELSE);
            parseStatement(); // Parse the statement inside the else block.
            icg.addInstruction("L3:");
        }
        else
        {
            icg.addInstruction("L2:");
        }
    }
    /*
        parseReturnStatement handles the parsing of return statements.
        It expects the keyword return, followed by an expression to return, and a semicolon to terminate the statement.
        It generates intermediate code to represent the return of the expression.
        Example:
        return x + 5;   -->  This will generate intermediate code like return x + 5.
    */
    void parseReturnStatement()
    {
        expect(T_RETURN);
        string expr = parseExpression();
        icg.addInstruction("return " + expr); // Generate intermediate code for the return statement.
        expect(T_SEMICOLON);
    }
    /*
        parseBlock handles the parsing of block statements, which are enclosed in curly braces { }.
        It parses the statements inside the block recursively until it reaches the closing brace.
        Example:
        { x = 10; y = 20; }   -->  This will parse each statement inside the block.
    */
    void parseBlock()
    {

        expect(T_LBRACE); // Expect and consume the opening brace {.
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseStatement(); // Parse the statements inside the block.
        }
        expect(T_RBRACE);
    }
    /*
        parseExpression handles the parsing of expressions involving addition, subtraction, or comparison operations.
        It first parses a term, then processes addition (+) or subtraction (-) operators if present, generating
        intermediate code for the operations.
        Example:
        5 + 3 - 2;  -->  This will generate intermediate code like t0 = 5 + 3 and t1 = t0 - 2.
    */
    string parseExpression()
    {
        string term = parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS)
        {
            TokenType op = tokens[pos++].type;
            string nextTerm = parseTerm();                                                       // Parse the next term in the expression.
            string temp = icg.newTemp();                                                         // Generate a temporary variable for the result
            icg.addInstruction(temp + " = " + term + (op == T_PLUS ? " + " : " - ") + nextTerm); // Intermediate code for operation
            term = temp;
        }
        if (tokens[pos].type == T_GT)
        {
            pos++;
            string nextExpr = parseExpression();                        // Parse the next expression for the comparison.
            string temp = icg.newTemp();                                // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = " + term + " > " + nextExpr); // Intermediate code for the comparison.
            term = temp;
        }
        return term;
    }
    /*
        parseTerm handles the parsing of terms involving multiplication or division operations.
        It first parses a factor, then processes multiplication (*) or division (/) operators if present,
        generating intermediate code for the operations.
        Example:
        5 * 3 / 2;   This will generate intermediate code like t0 = 5 * 3 and t1 = t0 / 2.
    */
    string parseTerm()
    {
        string factor = parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            TokenType op = tokens[pos++].type;
            string nextFactor = parseFactor();
            string temp = icg.newTemp();                                                            // Generate a temporary variable for the result.
            icg.addInstruction(temp + " = " + factor + (op == T_MUL ? " * " : " / ") + nextFactor); // Intermediate code for operation.
            factor = temp;                                                                          // Update the factor to be the temporary result.
        }
        return factor;
    }
    /*
        parseFactor handles the parsing of factors in expressions, which can be either numeric literals, identifiers
        (variables), or expressions inside parentheses (for sub-expressions).
        Example:
        5;          -->  This will return the number "5".
        x;          -->  This will return the identifier "x".
        (5 + 3);    --> This will return the sub-expression "5 + 3".
    */
    string parseFactor()
    {
        if (tokens[pos].type == T_NUM)
        {
            return tokens[pos++].value;
        }
        else if (tokens[pos].type == T_ID)
        {
            return tokens[pos++].value;
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            expect(T_LPAREN);
            string expr = parseExpression();
            expect(T_RPAREN);
            return expr;
        }
        else
        {
            cout << "Syntax error: unexpected token '" << tokens[pos].value << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
    }
    /*
        expect function:
        This functin is used to check whether the current token matches the expected type.
        If the token type does not match the expected type, an error message is displayed
        and the program exits. If the token type matches, it advances the position to the next token.
    */
    void expect(TokenType type)
    {
        if (tokens[pos].type != type)
        {
            cout << "Syntax error: expected '" << type << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
        pos++;
    }
    /*
    Explanation:
    - The expect function ensures that the parser encounters the correct tokens in the expected order.
    - It's mainly used for non-value-based tokens, such as keywords, operators, and delimiters (e.g., semicolons).
    - If the parser encounters an unexpected token, it halts the process by printing an error message, indicating where the error occurred (line number) and what was expected.
    - The pos++ advances to the next token after confirming the expected token is present.

    Use Case:
    - This function is helpful when checking for the correct syntax or structure in a language's grammar, ensuring the parser processes the tokens in the correct order.
    */
    string expectAndReturnValue(TokenType type)
    {
        string value = tokens[pos].value;
        expect(type);
        return value;
    }

    /*
        Why both functions are needed:
        - The expect function is useful when you are only concerned with ensuring the correct token type without needing its value.
        - For example, ensuring a semicolon ; or a keyword if is present in the source code.
        - The expectAndReturnValue function is needed when the parser not only needs to check for a specific token but also needs to use the value of that token in the next stages of compilation or interpretation.
        - For example, extracting the name of a variable (T_ID) or the value of a constant (T_NUMBER) to process it in a symbol table or during expression evaluation.
    */
};

// class AssemblyGenerator
// {
// public:
//     void generate(const vector<string> &intermediateCode)
//     {
//         for (const auto &instr : intermediateCode)
//         {
//             vector<string> tokens = tokenize(instr);
//             if (tokens.size() == 3 && tokens[1] == "=") // Assignment
//             {
//                 if (isNumber(tokens[2]))
//                 {
//                     cout << "MOV " << tokens[0] << ", #" << tokens[2] << endl; // Direct assignment
//                 }
//                 else
//                 {
//                     cout << "MOV " << tokens[0] << ", " << tokens[2] << endl; // Copy value
//                 }
//             }
//             else if (tokens.size() == 5 && tokens[3] == "+") // Addition
//             {
//                 cout << "MOV R1, " << tokens[2] << endl;
//                 cout << "ADD R1, " << tokens[4] << endl;
//                 cout << "MOV " << tokens[0] << ", R1" << endl;
//             }
//             else if (tokens.size() == 5 && tokens[3] == "-") // Subtraction
//             {
//                 cout << "MOV R1, " << tokens[2] << endl;
//                 cout << "SUB R1, " << tokens[4] << endl;
//                 cout << "MOV " << tokens[0] << ", R1" << endl;
//             }
//             else if (tokens.size() == 5 && tokens[3] == "*") // Multiplication
//             {
//                 cout << "MOV R1, " << tokens[2] << endl;
//                 cout << "MUL R1, " << tokens[4] << endl;
//                 cout << "MOV " << tokens[0] << ", R1" << endl;
//             }
//             else if (tokens.size() == 5 && tokens[3] == "/") // Division
//             {
//                 cout << "MOV R1, " << tokens[2] << endl;
//                 cout << "DIV R1, " << tokens[4] << endl;
//                 cout << "MOV " << tokens[0] << ", R1" << endl;
//             }
//             else if (tokens.size() == 3 && tokens[0] == "if") // Conditional jump
//             {
//                 cout << "CMP " << tokens[1] << ", #0" << endl;
//                 cout << "JEQ " << tokens[2] << endl;
//             }
//             else if (tokens[0] == "goto") // Unconditional jump
//             {
//                 cout << "JMP " << tokens[1] << endl;
//             }
//             else if (tokens[0].back() == ':') // Label
//             {
//                 cout << tokens[0] << endl;
//             }
//             else if (tokens.size() == 2 && tokens[0] == "return") // Return statement
//             {
//                 cout << "MOV R0, " << tokens[1] << endl;
//                 cout << "RET" << endl;
//             }
//             else
//             {
//                 cerr << "Unknown instruction: " << instr << endl;
//                 exit(1);
//             }
//         }
//     }
// private:
//     vector<string> tokenize(const string &instr)
//     {
//         vector<string> tokens;
//         size_t start = 0;
//         for (size_t i = 0; i <= instr.size(); ++i)
//         {
//             if (i == instr.size() || isspace(instr[i]))
//             {
//                 if (i > start)
//                 {
//                     tokens.push_back(instr.substr(start, i - start));
//                 }
//                 start = i + 1;
//             }
//         }
//         return tokens;
//     }
//     bool isNumber(const string &str)
//     {
//         return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
//     }
// };





























// class AssemblyCodeGenerator
// {
// private:
//     vector<string> assemblyCode;
//     int labelCounter;
//     unordered_map<string, string> variableRegisterMap;
//     int registerCounter = 0;

//     string allocateRegister(const string &var)
//     {
//         if (variableRegisterMap.find(var) == variableRegisterMap.end())
//         {
//             variableRegisterMap[var] = "r" + to_string(registerCounter++);
//         }
//         return variableRegisterMap[var];
//     }

//     string generateLabel()
//     {
//         return "L" + to_string(labelCounter++);
//     }

//     void addInstruction(const string &instruction)
//     {
//         assemblyCode.push_back(instruction);
//     }

//     void processAssignment(const string &line)
//     {
//         auto eqPos = line.find("=");
//         string tVar = line.substr(0, eqPos);
//         string value = line.substr(eqPos + 1);

//         tVar = trim(tVar);
//         value = trim(value);

//         string targetRegister = allocateRegister(tVar);

//         // Direct assignment
//         if (isdigit(value[0]))
//         {
//             addInstruction("li " + targetRegister + ", " + value);
//         }
//         else
//         {
//             string sourceRegister = allocateRegister(value);
//             addInstruction("move " + targetRegister + ", " + sourceRegister);
//         }
//     }

//     void processBinaryOperation(const string &line)
//     {
//         static const unordered_map<string, string> operationMap = {
//             {"+", "add"},
//             {"-", "sub"},
//             {"*", "mul"},
//             {"/", "div"}};

//         auto eqPos = line.find("=");
//         string tVar = line.substr(0, eqPos);
//         string expr = line.substr(eqPos + 1);

//         string op;
//         for (const auto &entry : operationMap)
//         {
//             if (expr.find(entry.first) != string::npos)
//             {
//                 op = entry.first;
//                 break;
//             }
//         }

//         auto opPos = expr.find(op);
//         string lhs = expr.substr(0, opPos);
//         string rhs = expr.substr(opPos + 1);

//         tVar = trim(tVar);
//         lhs = trim(lhs);
//         rhs = trim(rhs);

//         string targetRegister = allocateRegister(tVar);
//         string lhsRegister = isdigit(lhs[0]) ? allocateRegister(generateTempRegister(lhs)) : allocateRegister(lhs);
//         string rhsRegister = isdigit(rhs[0]) ? allocateRegister(generateTempRegister(rhs)) : allocateRegister(rhs);

//         if (isdigit(lhs[0]))
//             addInstruction("li " + lhsRegister + ", " + lhs);
//         if (isdigit(rhs[0]))
//             addInstruction("li " + rhsRegister + ", " + rhs);

//         addInstruction(operationMap.at(op) + " " + targetRegister + ", " + lhsRegister + ", " + rhsRegister);
//     }

//     void processRelationalOperation(const string &line)
//     {
//         auto eqPos = line.find("=");
//         string tVar = line.substr(0, eqPos);
//         string expr = line.substr(eqPos + 1);

//         tVar = trim(tVar);

//         string op;
//         if (expr.find(">=") != string::npos)
//             op = ">=";
//         else if (expr.find("<=") != string::npos)
//             op = "<=";
//         else if (expr.find("==") != string::npos)
//             op = "==";
//         else if (expr.find("!=") != string::npos)
//             op = "!=";
//         else if (expr.find(">") != string::npos)
//             op = ">";
//         else if (expr.find("<") != string::npos)
//             op = "<";

//         auto opPos = expr.find(op);
//         string lhs = expr.substr(0, opPos);
//         string rhs = expr.substr(opPos + op.size());

//         lhs = trim(lhs);
//         rhs = trim(rhs);

//         string lhsReg = isdigit(lhs[0]) ? allocateRegister(generateTempRegister(lhs)) : allocateRegister(lhs);
//         string rhsReg = isdigit(rhs[0]) ? allocateRegister(generateTempRegister(rhs)) : allocateRegister(rhs);
//         string resultReg = allocateRegister(tVar);

//         if (isdigit(lhs[0]))
//             addInstruction("li " + lhsReg + ", " + lhs);
//         if (isdigit(rhs[0]))
//             addInstruction("li " + rhsReg + ", " + rhs);

//         if (op == ">")
//         {
//             addInstruction("sgt " + resultReg + ", " + lhsReg + ", " + rhsReg);
//         }
//         else if (op == "<")
//         {
//             addInstruction("slt " + resultReg + ", " + lhsReg + ", " + rhsReg);
//         }
//         else if (op == ">=")
//         {
//             addInstruction("slt " + resultReg + ", " + lhsReg + ", " + rhsReg);
//             addInstruction("seqz " + resultReg + ", " + resultReg);
//         }
//         else if (op == "<=")
//         {
//             addInstruction("sgt " + resultReg + ", " + lhsReg + ", " + rhsReg);
//             addInstruction("seqz " + resultReg + ", " + resultReg);
//         }
//         else if (op == "==")
//         {
//             addInstruction("sub " + resultReg + ", " + lhsReg + ", " + rhsReg);
//             addInstruction("seqz " + resultReg + ", " + resultReg);
//         }
//         else if (op == "!=")
//         {
//             addInstruction("sub " + resultReg + ", " + lhsReg + ", " + rhsReg);
//             addInstruction("snez " + resultReg + ", " + resultReg);
//         }
//     }

//     void processConditional(const string &line)
//     {
//         auto ifPos = line.find("if");
//         auto gotoPos = line.find("goto");
//         string condition = line.substr(ifPos + 2, gotoPos - (ifPos + 2));
//         string label = line.substr(gotoPos + 4);

//         condition = trim(condition);
//         label = trim(label);

//         string conditionReg = allocateRegister(condition);
//         addInstruction("bne " + conditionReg + ", x0, " + label);
//     }

//     void processGoto(const string &line)
//     {
//         auto gotoPos = line.find("goto");
//         string label = line.substr(gotoPos + 4);
//         label = trim(label);

//         addInstruction("jmp " + label);
//     }

//     void processLabel(const string &line)
//     {
//         addInstruction(line);
//     }

//     string generateTempRegister(const string &value)
//     {
//         return "temp_" + value;
//     }

//     string trim(const string &str)
//     {
//         auto start = str.find_first_not_of(" \t");
//         auto end = str.find_last_not_of(" \t");
//         return (start == string::npos) ? "" : str.substr(start, end - start + 1);
//     }

//     ///////////////////////////////////////////////
//     /* Additional feature for Arrays */
//     void processArrayDeclaration(const string &line)
//     {
//         // Handling array declaration (e.g., int arr[5];)
//         auto arrPos = line.find("[]");
//         if (arrPos != string::npos)
//         {
//             string arrName = line.substr(0, arrPos);
//             arrName = trim(arrName);
//             int arraySize = stoi(line.substr(arrPos + 2)); // Read array size (e.g., arr[5])

//             // Allocate memory for the array (Assume each element is 4 bytes for int)
//             addInstruction("allocate " + arrName + ", " + to_string(arraySize * 4));
//         }
//     }

//     void processArrayAccess(const string &line)
//     {
//         // Accessing array element (e.g., arr[2])
//         auto arrPos = line.find("[]");
//         if (arrPos != string::npos)
//         {
//             string arrName = line.substr(0, arrPos);
//             string index = line.substr(arrPos + 2); // Read index value (e.g., 2)

//             arrName = trim(arrName);
//             index = trim(index);

//             string arrRegister = allocateRegister(arrName);
//             string indexRegister = allocateRegister(index);
//             string resultRegister = allocateRegister("tempResult");

//             // Calculate memory address (index * size_of_element)
//             addInstruction("mul " + indexRegister + ", " + indexRegister + ", 4");               // Assume each element is 4 bytes
//             addInstruction("add " + resultRegister + ", " + arrRegister + ", " + indexRegister); // Add base address

//             // Load value from the array into resultRegister
//             addInstruction("lw " + resultRegister + ", 0(" + resultRegister + ")");
//         }
//     }

//     string generateTempRegister(const string &value)
//     {
//         return "temp_" + value;
//     }

//     string trim(const string &str)
//     {
//         auto start = str.find_first_not_of(" \t");
//         auto end = str.find_last_not_of(" \t");
//         return (start == string::npos) ? "" : str.substr(start, end - start + 1);
//     }

//     //////////////////////////////////////////////////////////////








// public:
//     AssemblyCodeGenerator() : labelCounter(0) {}

//     void generateAssembly(const vector<string> &intermediateCode)
//     {
//         for (const auto &line : intermediateCode)
//         {
//             string Line = trim(line);
//             if (Line.empty())
//                 continue;

//             if (Line.find("=") != string::npos)
//             {
//                 if (Line.find(">") != string::npos || Line.find("<") != string::npos ||
//                     Line.find("==") != string::npos || Line.find(">=") != string::npos ||
//                     Line.find("<=") != string::npos || Line.find("!=") != string::npos)
//                 {
//                     processRelationalOperation(Line);
//                 }
//                 else if (Line.find("+") != string::npos ||
//                          Line.find("-") != string::npos ||
//                          Line.find("*") != string::npos ||
//                          Line.find("/") != string::npos)
//                 {
//                     processBinaryOperation(Line);
//                 }
//                 else
//                 {
//                     processAssignment(Line);
//                 }
//             }
//             else if (Line.find("if") == 0)
//             {
//                 processConditional(Line);
//             }
//             else if (Line.find("goto") == 0)
//             {
//                 processGoto(Line);
//             }
//             else if (Line.back() == ':')
//             {
//                 processLabel(Line);
//             }
//             else if (Line.find("[]") != string::npos)
//             {
//                 processArrayAccess(Line); // Process array access
//             }
//         }
//     }

//     void printAssemblyCode()
//     {
//         cout << "\n\n=============================" << endl;
//         cout << "ASSEMBLY CODE:" << endl;
//         cout << "=============================" << endl;
//         for (const auto &instruction : assemblyCode)
//         {
//             cout << instruction << "\n";
//         }
//         cout << "\n\n";
//     }
// };


class AssemblyCodeGenerator
{
private:
    vector<string> assemblyCode;
    int labelCounter;
    unordered_map<string, string> variableRegisterMap;
    int registerCounter = 0;

    // Allocate a register for variables and arrays
    string allocateRegister(const string &var)
    {
        if (variableRegisterMap.find(var) == variableRegisterMap.end())
        {
            variableRegisterMap[var] = "r" + to_string(registerCounter++);
        }
        return variableRegisterMap[var];
    }

    // Add assembly instruction to the list
    void addInstruction(const string &instruction)
    {
        assemblyCode.push_back(instruction);
    }

    // Helper function to trim whitespace
    string trim(const string &str)
    {
        auto start = str.find_first_not_of(" \t");
        auto end = str.find_last_not_of(" \t");
        return (start == string::npos) ? "" : str.substr(start, end - start + 1);
    }

    // Process array declaration (e.g., arr[5];)
    void processArrayDeclaration(const string &line)
    {
        auto arrPos = line.find("[");
        if (arrPos != string::npos)
        {
            string arrName = line.substr(0, arrPos);
            arrName = trim(arrName);

            // Get the array size (e.g., arr[5])
            string arraySizeStr = line.substr(arrPos + 1, line.find("]") - arrPos - 1);
            int arraySize = stoi(arraySizeStr);

            // Assuming each element is 4 bytes (for int)
            addInstruction("allocate " + arrName + ", " + to_string(arraySize * 4));
        }
    }

    // Process array access (e.g., arr[2] = 10;)
    void processArrayAccess(const string &line)
    {
        auto arrPos = line.find("[");
        if (arrPos != string::npos)
        {
            string arrName = line.substr(0, arrPos);
            string index = line.substr(arrPos + 1, line.find("]") - arrPos - 1);
            arrName = trim(arrName);
            index = trim(index);

            string arrRegister = allocateRegister(arrName);  // Base address of the array
            string indexRegister = allocateRegister(index); // Index value
            string resultRegister = allocateRegister("tempResult"); // Register to store result

            // Calculate the memory address (index * 4 bytes for an int)
            addInstruction("mul " + indexRegister + ", " + indexRegister + ", 4"); // index * 4 (size of int)
            addInstruction("add " + resultRegister + ", " + arrRegister + ", " + indexRegister); // Base + index_offset

            // For array write, store value into the array element
            if (line.find("=") != string::npos)
            {
                string value = line.substr(line.find("=") + 1);
                value = trim(value);
                addInstruction("li " + resultRegister + ", " + value); // Load value to resultRegister
                addInstruction("sw " + resultRegister + ", 0(" + resultRegister + ")"); // Store value into array
            }
            else
            {
                // For array read, load the value into the result register
                addInstruction("lw " + resultRegister + ", 0(" + resultRegister + ")");
            }
        }
    }

    // Process assignments (e.g., x = 10;)
    void processAssignment(const string &line)
    {
        auto eqPos = line.find("=");
        string tVar = line.substr(0, eqPos);
        string value = line.substr(eqPos + 1);

        tVar = trim(tVar);
        value = trim(value);

        string targetRegister = allocateRegister(tVar);

        // Direct assignment
        if (isdigit(value[0]))
        {
            addInstruction("li " + targetRegister + ", " + value);
        }
        else
        {
            string sourceRegister = allocateRegister(value);
            addInstruction("move " + targetRegister + ", " + sourceRegister);
        }
    }

    // Process binary operations (e.g., x = a + b;)
    void processBinaryOperation(const string &line)
    {
        static const unordered_map<string, string> operationMap = {
            {"+", "add"},
            {"-", "sub"},
            {"*", "mul"},
            {"/", "div"}};

        auto eqPos = line.find("=");
        string tVar = line.substr(0, eqPos);
        string expr = line.substr(eqPos + 1);

        string op;
        for (const auto &entry : operationMap)
        {
            if (expr.find(entry.first) != string::npos)
            {
                op = entry.first;
                break;
            }
        }

        auto opPos = expr.find(op);
        string lhs = expr.substr(0, opPos);
        string rhs = expr.substr(opPos + 1);

        tVar = trim(tVar);
        lhs = trim(lhs);
        rhs = trim(rhs);

        string targetRegister = allocateRegister(tVar);
        string lhsRegister = isdigit(lhs[0]) ? allocateRegister("temp_" + lhs) : allocateRegister(lhs);
        string rhsRegister = isdigit(rhs[0]) ? allocateRegister("temp_" + rhs) : allocateRegister(rhs);

        if (isdigit(lhs[0]))
            addInstruction("li " + lhsRegister + ", " + lhs);
        if (isdigit(rhs[0]))
            addInstruction("li " + rhsRegister + ", " + rhs);

        addInstruction(operationMap.at(op) + " " + targetRegister + ", " + lhsRegister + ", " + rhsRegister);
    }

public:
    AssemblyCodeGenerator() : labelCounter(0) {}

    // Generate the assembly code from the intermediate code
    void generateAssembly(const vector<string>& intermediateCode)
    {
        for (const string& line : intermediateCode)
        {
            string trimmedLine = trim(line);
            if (trimmedLine.empty())
                continue;

            // Handle different types of lines
            if (trimmedLine.find("=") != string::npos)
            {
                if (trimmedLine.find("[") != string::npos) // Array access
                {
                    processArrayAccess(trimmedLine);
                }
                else if (trimmedLine.find("+") != string::npos || trimmedLine.find("-") != string::npos ||
                         trimmedLine.find("*") != string::npos || trimmedLine.find("/") != string::npos)
                {
                    processBinaryOperation(trimmedLine);
                }
                else
                {
                    processAssignment(trimmedLine);
                }
            }
            else if (trimmedLine.find("[") != string::npos) // Array declaration
            {
                processArrayDeclaration(trimmedLine);
            }
        }
    }

    // Function to get the generated assembly code
    const vector<string>& getAssemblyCode() const
    {
        return assemblyCode;
    }

    
    void printAssemblyCode()
    {
        cout << "\n\n=============================" << endl;
        cout << "ASSEMBLY CODE:" << endl;
        cout << "=============================" << endl;
        for (const auto &instruction : assemblyCode)
        {
            cout << instruction << "\n";
        }
        cout << "\n\n";
    }
};


int main()
{
    string src = R"(
    int x;
    x = 10;
    int y;
    y = 20;
    int sum;
    sum = x + y * 3;
    
    if(5 > 3){
        x = 20;
    }
    while(true){
        x = x +1;
    }
    )";
    Lexer lexer(src);
    vector<Token> tokens = lexer.tokenize();

    SymbolTable symTable;
    IntermediateCodeGnerator icg;
    Parser parser(tokens, symTable, icg);

    parser.parseProgram();
    icg.printInstructions();

    AssemblyCodeGenerator acg;
    acg.generateAssembly(icg.instructions);
    acg.printAssemblyCode();



    /* Section for Arrays feature */

    // Example intermediate code for arrays and assignment
    vector<string> intermediateCode = {
        "arr[5];",           // Array declaration with 5 elements
        "arr[0] = 10;",      // Array element access and assignment
        "arr[1] = 20;",      // Array element access and assignment
        // "arr[2] = arr[0] + arr[1];", // Array element access with binary operation
        // "x = arr[2];"        // Assignment of array element to a variable
    };

    // AssemblyCodeGenerator acg;
    acg.generateAssembly(intermediateCode);

    // Print the generated assembly code
    const vector<string>& assemblyCode = acg.getAssemblyCode();
    cout << "Generated Assembly Code With Arrays:" << endl;
    for (const string& instruction : assemblyCode)
    {
        cout << instruction << endl;
    }

    return 0;
}

// code understanding request