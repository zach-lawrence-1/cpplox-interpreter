#ifndef AST
#define AST

#include <memory>

#include "scanner.h"
#include "interpreterObject.h"

//TODO: rewrite implementation using smart pointers
//NOTE: may need to derive Expr from std::enable_shared_from_this

class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor
{
    public:
        virtual InterpreterObject visitBinaryExpr(Binary& expr) = 0;
        virtual InterpreterObject visitGroupingExpr(Grouping& expr) = 0;
        virtual InterpreterObject visitLiteralExpr(Literal& expr) = 0;
        virtual InterpreterObject visitUnaryExpr(Unary& expr) = 0;
};

class Expr
{
    public:
        virtual InterpreterObject accept(Visitor& visitor) = 0;
        virtual ~Expr() = default;
};

class Binary : public Expr
{
    public:
        std::unique_ptr<Expr> m_left;
        Token m_oper;
        std::unique_ptr<Expr> m_right;

    public:
        Binary(std::unique_ptr<Expr> left, Token& oper, std::unique_ptr<Expr> right);
        InterpreterObject accept(Visitor& visitor) override;
};

class Grouping : public Expr
{
    public:
        std::unique_ptr<Expr> m_expression;

    public:
        Grouping(std::unique_ptr<Expr> expression);
        InterpreterObject accept(Visitor& visitor) override;
};

class Literal : public Expr
{
    public:
        InterpreterObject m_value;

    public:
        Literal(InterpreterObject& value);
        InterpreterObject accept(Visitor& visitor) override;
};

class Unary : public Expr
{
    public:
        Token m_oper;
        std::unique_ptr<Expr> m_right;

    public:
        Unary(Token& oper, std::unique_ptr<Expr> right);
        InterpreterObject accept(Visitor& visitor) override;
};

#endif