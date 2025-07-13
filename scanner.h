#ifndef SCANNER
#define SCANNER

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

enum TokenType
{
    //single-character tokens.
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,

    //one or two character tokens.
    TOKEN_NOT,
    TOKEN_NOT_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,

    //literals.
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,

    //keywords.
    TOKEN_AND,
    TOKEN_CLASS,
    TOKEN_ELSE,
    TOKEN_FALSE,
    TOKEN_FOR,
    TOKEN_FUN,
    TOKEN_IF,
    TOKEN_NIL,
    TOKEN_OR,
    TOKEN_PRINT,
    TOKEN_RETURN,
    TOKEN_SUPER,
    TOKEN_THIS,
    TOKEN_TRUE,
    TOKEN_VAR,
    TOKEN_WHILE,

    TOKEN_ERROR,
    TOKEN_EOF
};

class Token
{
    private:
        TokenType m_type;
        std::string m_lexeme;
        int m_line;

    public:
        Token() = default;
        Token(TokenType type, std::string lexeme, int line);
        std::string getLexeme();
        TokenType getType();
};

class Scanner
{
    private:
        std::string m_source;
        std::vector<Token> m_tokens;
        std::unordered_map<std::string, TokenType> m_keywords = {
            {"and", TOKEN_AND},
            {"class", TOKEN_CLASS},
            {"else", TOKEN_ELSE},
            {"false", TOKEN_FALSE},
            {"for", TOKEN_FOR},
            {"fun", TOKEN_FUN},
            {"if", TOKEN_IF},
            {"nil", TOKEN_NIL},
            {"or", TOKEN_OR},
            {"print", TOKEN_PRINT},
            {"return", TOKEN_RETURN},
            {"super", TOKEN_SUPER},
            {"this", TOKEN_THIS},
            {"true", TOKEN_TRUE},
            {"var", TOKEN_VAR},
            {"while", TOKEN_WHILE}

        };
        int m_start = 0;
        int m_current = 0;
        int m_line = 1;

    public:
        Scanner(const std::string& source);
        std::vector<Token> scanTokens();
        void scanToken();
        char advance();
        void addToken(TokenType type, std::string stringValue);
        bool match(const char& expected);
        char peekChar();
        char peekNextChar();
        void processString();
        bool isDigit(const char& character);
        void processNumber();
        bool isAlpha(const char& character);
        bool isAlphaNumeric(const char& character);
        void processIdentifier();
};

#endif