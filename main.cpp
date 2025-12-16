#include<string>
#include<vector>
#include<cctype>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>

using namespace std;

enum TokenType {
    LBRACKET, RBRACKET, // [, ]
    SEMI, // ;
    PLUS, MINUS, STAR, // +, -, *
    SLASH, // /

    ASSIGN, // ->
    EQUALS, // ==
    GREATER, GREATER_EQUAL, // >, >=
    LESS, LESS_EQUAL, // <, <=

    IDENTIFIER, // ~abc
    STRING, // 'hello'
    NUMBER, // 123

    IF, // ?
    ELSE, // :
    WHILE, // @
    PRINT, // !

    END_OF_FILE,
};

struct Token {
    TokenType type;
    string lexeme;
    int line;
};

string tokenTypeToString(TokenType type) {
    switch (type) {
        case LBRACKET: return "LBRACKET";
        case RBRACKET: return "RBRACKET";
        case SEMI:     return "SEMI";
        case PLUS:     return "PLUS";
        case MINUS:    return "MINUS";
        case STAR:     return "STAR";
        case SLASH:    return "SLASH";
        case ASSIGN:   return "ASSIGN";
        case EQUALS:   return "EQUALS";
        case GREATER:  return "GREATER";
        case GREATER_EQUAL: return "GREATER_EQUAL";
        case LESS:     return "LESS";
        case LESS_EQUAL: return "LESS_EQUAL";
        case IDENTIFIER: return "IDENTIFIER";
        case STRING:   return "STRING";
        case NUMBER:   return "NUMBER";
        case IF:       return "IF";
        case ELSE:     return "ELSE";
        case WHILE:    return "WHILE";
        case PRINT:    return "PRINT";
        case END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}

class Scanner {
private:
    string source;
    vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

public:
    Scanner(string source) : source(source) {}

    vector<Token> scanTokens() {
        while(current < source.length()) {
            start = current;
            scanToken();
        }
        tokens.push_back({END_OF_FILE, "", line});
        return tokens;
    }

private:

    void scanToken() {
        char c = next();
        
        switch(c) {
            case '[':
                addToken(LBRACKET);
                break;
            case ']':
                addToken(RBRACKET);
                break;
            case ';':
                addToken(SEMI);
                break;

            case '+':
                addToken(PLUS);
                break;
            case '-':
                if (lookAhead() == '>') {
                    current++;
                    addToken(ASSIGN);
                } else addToken(MINUS);
                break;
            case '*':
                addToken(STAR);
                break;
            case '=':
                if (lookAhead() == '=') {
                    current++;
                    addToken(EQUALS);
                }
                // else "Error"
                break;

            case '?':
                addToken(IF);
                break;
            case ':':
                addToken(ELSE);
                break;
            case '@':
                addToken(WHILE);
                break;
            case '!':
                addToken(PRINT);
                break;

            case ' ':
            case '\t':
            case '\r':
                break;

            case '\n':
                line++;
                break;

            case '~':
                identifier();
                break;
            
            case '\"':
                stringLiteral();
                break;

            default:
                if (isdigit(c)) {
                    number();
                }
                // else "Error"
        }
    }

    void identifier() {
        while (isalpha(lookAhead())) next();
        addToken(IDENTIFIER);
    }

    void number() {
        while (isdigit(lookAhead())) next();
        addToken(NUMBER);
    }

    void stringLiteral() {
        while (lookAhead() != '\"') next();
        addToken(STRING);
        next();
    }

    char next() {
        return source[current++];
    }

    void addToken(TokenType token) {
        string lexeme = source.substr(start, current - start);
        tokens.push_back({token, lexeme, line});
    }

    char lookAhead() {
        if (current >= source.length()) return '\0';
        return source[current];
    }

};

string readFile(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "ERROR";
        exit(1);
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    string code = readFile(argv[1]);
    Scanner scanner(code);
    vector<Token> tokens = scanner.scanTokens();

    cout << left << setw(10) << "LINE" 
         << setw(20) << "TOKEN TYPE" 
         << setw(20) << "LEXEME" << endl;
    cout << string(50, '-') << endl;

    for (Token t : tokens) {
        cout << left << setw(10) << t.line 
             << setw(20) << tokenTypeToString(t.type) 
             << setw(20) << t.lexeme << endl;
    }

    return 0;
}
