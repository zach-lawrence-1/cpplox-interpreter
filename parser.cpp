#include "parser.h"
#include "execution.h"

Parser::Parser(std::vector<Token>& tokens)
{
    m_tokens = tokens;
    m_current = 0;
}

std::unique_ptr<Expr> Parser::expression()
{
    return equality();
}

std::unique_ptr<Expr> Parser::equality()
{
    std::unique_ptr<Expr> expr = comparison();
    std::vector<TokenType> tokensToMatch = {TOKEN_NOT_EQUAL, TOKEN_EQUAL_EQUAL};
    
    while (match({TOKEN_NOT_EQUAL, TOKEN_EQUAL_EQUAL}))
    {
        Token oper = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

bool Parser::match(const std::vector<TokenType>& types)
{
    for (TokenType type : types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }

    return false;
}

bool Parser::check(TokenType type)
{
    if (isAtEnd())
        return false;

    return peek().getType() == type;
}

Token Parser::advance()
{
    if (!isAtEnd())
        m_current++;
    
    return previous();
}

bool Parser::isAtEnd()
{
    return peek().getType() == TOKEN_EOF;
}

Token Parser::peek()
{
    return m_tokens.at(m_current);
}

Token Parser::previous()
{
    return m_tokens.at(m_current - 1);
}

std::unique_ptr<Expr> Parser::comparison()
{
    std::unique_ptr<Expr> expr = term();
    std::vector<TokenType> tokensToMatch = {TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_LESS, TOKEN_LESS_EQUAL};
    
    while (match(tokensToMatch))
    {
        Token oper = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::term()
{
    std::unique_ptr<Expr> expr = factor();
    std::vector<TokenType> tokensToMatch = {TOKEN_MINUS, TOKEN_PLUS};

    while (match(tokensToMatch))
    {
        Token oper = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor()
{
    std::unique_ptr<Expr> expr = unary();
    std::vector<TokenType> tokensToMatch = {TOKEN_SLASH, TOKEN_STAR};

    while (match(tokensToMatch))
    {
        Token oper = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary()
{
    std::vector<TokenType> tokensToMatch = {TOKEN_NOT, TOKEN_MINUS};

    if (match(tokensToMatch))
    {
        Token oper = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Unary>(Unary(oper, std::move(right)));
    }

    return primary();
}

std::unique_ptr<Expr> Parser::primary()
{
    if (match({TOKEN_FALSE}))
    {
        InterpreterObject iO(false);
        return std::make_unique<Literal>(iO);
    }
    else if (match({TOKEN_TRUE}))
    {
        InterpreterObject iO(true);
        return std::make_unique<Literal>(iO);
    }
    else if (match({TOKEN_NIL}))
    {
        InterpreterObject iO;
        return std::make_unique<Literal>(iO);
    }

    else if (match({TOKEN_NUMBER}))
    {
        InterpreterObject iO(std::stod(previous().getLexeme()));
        return std::make_unique<Literal>(iO);
    }

    else if (match({TOKEN_STRING}))
    {
        InterpreterObject iO(previous().getLexeme());
        return std::make_unique<Literal>(iO);
    }

    if (match({TOKEN_LEFT_PAREN}))
    {
        std::unique_ptr<Expr> expr = expression();
        consume(TOKEN_RIGHT_PAREN, "EXPECT: ')' after expression");
        return std::make_unique<Grouping>(std::move(expr));
    }

    error(peek(), "expected expression");
    return nullptr;
}

Token Parser::consume(TokenType type, std::string message)
{
    if (check(type))
        return advance();
    
    error(peek(), message);
    return peek();
}

void Parser::synchronize()
{
    advance();

    while (!isAtEnd())
    {
        if (previous().getType() == TOKEN_SEMICOLON)
            return;

        switch(peek().getType())
        {
            case TOKEN_CLASS:
                break;
            case TOKEN_FUN:
                break;
            case TOKEN_VAR:
                break;
            case TOKEN_FOR:
                break;
            case TOKEN_IF:
                break;
            case TOKEN_WHILE:
                break;
            case TOKEN_PRINT:
                break;
            case TOKEN_RETURN:
                return;
            default:
                return;
        }

        advance();
    }
}

std::unique_ptr<Expr> Parser::parse()
{
    //TODO: add a way to not exit the code in every case when a syntax error occurs, maybe use exceptions
    return expression();
}