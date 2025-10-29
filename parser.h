#ifndef PARSER
#define PARSER

#include <memory>

#include "scanner.h"
#include "ast.h"

class Parser {
    private:
        std::vector<Token> m_tokens;
        int m_current;
    
    public:
        Parser(std::vector<Token>& tokens);
        std::unique_ptr<Expr> expression();
        std::unique_ptr<Expr> equality();
        bool match(const std::vector<TokenType>& types);
        bool check(TokenType type);
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        std::unique_ptr<Expr> comparison();
        std::unique_ptr<Expr> term();
        std::unique_ptr<Expr> factor();
        std::unique_ptr<Expr> unary();
        std::unique_ptr<Expr> primary();
        Token consume(TokenType type, std::string message);
        void synchronize();
        std::unique_ptr<Expr> parse();
};

#endif