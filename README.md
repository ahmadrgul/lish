# LISH Scanner (LISH Is Symbol Heaviness)

A custom Lexical Analyzer built in C++ for **LISH**, a symbol-heavy, logic-based programming language. This project was created as a part of the Compiler Construction coursework.

## ⚡ About LISH
LISH is a "Symbolist" language that prioritizes dense, logical notation over English keywords. 
* **Variables** are prefixed with `~` (e.g., `~count`).
* **Keywords** are single symbols (`?` for If, `@` for While).
* **Assignment** uses the arrow operator `->`.

## 📚 Lexical Syntax Specification

The LISH scanner recognizes the following tokens. Use this guide to write valid LISH programs.

### 1. Identifiers & Literals
| Token Type | Pattern / Rule | Examples |
| :--- | :--- | :--- |
| **Identifier** | Starts with `~` followed by alphanumeric chars | `~x`, `~count1`, `~temp` |
| **Number** | Sequence of digits | `10`, `500`, `0` |
| **String** | Text enclosed in double quotes | `"Hello World"`, `"Error 404"` |

### 2. Keywords (Control Flow)
LISH uses symbols instead of English words for control flow.

| Token | Symbol | Usage / Meaning |
| :--- | :--- | :--- |
| **IF** | `?` | Start a conditional check |
| **ELSE** | `:` | Alternative branch |
| **WHILE** | `@` | Start a loop |
| **PRINT** | `!` | Output to console |

### 3. Operators
| Token | Symbol | Description |
| :--- | :--- | :--- |
| **ASSIGN** | `->` | Assigns value to variable (e.g., `10 -> ~x`) |
| **EQUALS** | `==` | Equality comparison |
| **PLUS** | `+` | Addition |
| **MINUS** | `-` | Subtraction |
| **STAR** | `*` | Multiplication |
| **SLASH** | `/` | Division |
| **COMPARE**| `<` `<=` `>` `>=` | Comparison operators |

### 4. Delimiters
| Token | Symbol | Description |
| :--- | :--- | :--- |
| **BLOCK** | `[` and `]` | Starts and ends a block of code |
| **End Stmt**| `;` | Terminates a statement |

## 🚀 Features
* **Hand-written Scanner:** No external tools like Lex or Flex used.
* **Maximal Munch:** correctly distinguishes between `>` and `>=`.
* **Error Handling:** Detects unterminated strings and unknown characters.
* **Formatted Output:** Prints a clean table of Tokens, Lexemes, and Line numbers.

## 🛠️ Installation & Usage

### Prerequisites
* A C++ Compiler (g++ or clang)

### How to Run
1.  **Clone the repository**
    ```bash
    git clone [https://github.com/ahmadrgul/lish.git](https://github.com/ahmadrgul/lish.git)
    cd lish-scanner
    ```

2.  **Compile the code**
    ```bash
    g++ main.cpp -o lish
    ```

3.  **Run with a source file**
    Create a file named `source.lish` with your code, then run:
    ```bash
    ./lish source.lish
    ```

## 📝 Example Code
Here is what valid LISH code looks like:

```text
~x -> 10;           
@ ~x > 0 [          
    ? ~x == 5 [     
        ! "Halfway there!";
    ]
    ~x - 1 -> ~x;
]
