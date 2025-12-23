#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

  ERROR_TOKEN, // For errors
  END_OF_FILE,
};

struct Token {
  TokenType type;
  string lexeme;
  int line;
};

struct ScanError {
  int line;
  string message;
  string lexeme;
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
  vector<ScanError> errors;
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

  vector<ScanError> getErrors() { return errors; }

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
      } else {
        addError("Invalid operator '='. Use '==' or '->'.");
        addToken(ERROR_TOKEN);
      }
      break;

    case '<':
      if (lookAhead() == '=') {
        current++;
        addToken(LESS_EQUAL);
                } else addToken(LESS);
      break;
    case '>':
      if (lookAhead() == '=') {
        current++;
        addToken(GREATER_EQUAL);
                } else addToken(GREATER);
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
      } else {
        string ch(1, c);
        addError("Unknown character '" + ch + "'");
        addToken(ERROR_TOKEN);
      }
    }
  }

  void identifier() {
    if (!isalpha(lookAhead()) && !isdigit(lookAhead())) {
      addError("Invalid identifier: '~' must be followed by characters");
      addToken(ERROR_TOKEN);
      return;
    }
    while (isalnum(lookAhead()))
      next();
    addToken(IDENTIFIER);
  }

  void number() {
        while (isdigit(lookAhead())) next();
    addToken(NUMBER);
  }

  void stringLiteral() {
    int startLine = line;
    while (lookAhead() != '\"' && current < source.length()) {
      if (lookAhead() == '\n')
        line++;
      next();
    }
    if (current >= source.length()) {
      addError("Unterminated string starting at line " + to_string(startLine));
      addToken(ERROR_TOKEN);
      return;
    }
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

  void addError(string message) {
    string lexeme = source.substr(start, current - start);
    errors.push_back({line, message, lexeme});
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

string getOutputPath(string inputPath) {
  size_t lastDot = inputPath.find_last_of('.');
  if (lastDot != string::npos)
    return inputPath.substr(0, lastDot) + "_output.txt";
  return inputPath + "_output.txt";
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <source_file> [output_file]" << endl;
    return 1;
  }
  string inputPath = argv[1];
  string outputPath = (argc >= 3) ? argv[2] : getOutputPath(inputPath);
  string code = readFile(inputPath);
  Scanner scanner(code);
  vector<Token> tokens = scanner.scanTokens();
  vector<ScanError> errors = scanner.getErrors();

  ofstream outFile(outputPath);
  outFile << "LISH LEXICAL ANALYSIS" << endl << string(50, '=') << endl << endl;
  if (!errors.empty()) {
    outFile << "ERRORS FOUND: " << errors.size() << endl
            << string(50, '-') << endl;
    for (ScanError err : errors)
      outFile << "[Line " << err.line << "] " << err.message << endl;
    outFile << string(50, '-') << endl << endl;
  } else {
    outFile << "No errors found." << endl << endl;
  }
  outFile << left << setw(10) << "LINE" << setw(20) << "TOKEN TYPE" << setw(20)
          << "LEXEME" << endl;
  outFile << string(50, '-') << endl;
  for (Token t : tokens) {
    outFile << left << setw(10) << t.line << setw(20)
            << tokenTypeToString(t.type) << setw(20) << t.lexeme << endl;
  }
  outFile.close();

  cout << "Input:  " << inputPath << endl;
  cout << "Output: " << outputPath << endl;
  cout << "Tokens: " << tokens.size() << ", Errors: " << errors.size() << endl;
  if (!errors.empty()) {
    cout << endl << "Errors:" << endl;
    for (ScanError err : errors)
      cout << "  [Line " << err.line << "] " << err.message << endl;
  }
  return errors.empty() ? 0 : 1;
}
