#ifndef AST
#define AST

#include "scanner.h"
#include "interpreterObject.h"

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
};

class Binary : public Expr
{
    public:
        Expr* m_left;
        Token m_oper;
        Expr* m_right;

    public:
        Binary(Expr& left, Token oper, Expr& right);
        InterpreterObject accept(Visitor& visitor) override;
};

class Grouping : public Expr
{
    public:
        Expr* m_expression;

    public:
        Grouping(Expr& expression);
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
        Expr* m_right;

    public:
        Unary(const Token& oper, Expr& right);
        InterpreterObject accept(Visitor& visitor) override;
};

#endif