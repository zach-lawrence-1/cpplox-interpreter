#ifndef AST
#define AST

#include "scanner.h"

template<typename T>
class Binary;

template<typename T>
class Grouping;

template<typename T>
class Literal;

template<typename T>
class Unary;

template<typename T>
class Visitor
{
    public:
        virtual T visitBinaryExpr(Binary<T>& expr) = 0;
        virtual T visitGroupingExpr(Grouping<T>& expr) = 0;
        virtual T visitLiteralExpr(Literal<T>& expr) = 0;
        virtual T visitUnaryExpr(Unary<T>& expr) = 0;
};

template<typename T>
class Expr
{
    public:
        virtual T accept(Visitor<T>& visitor) = 0;
};

template<typename T>
class Binary : public Expr<T>
{
    private:
        const Expr<T> m_left;
        const Token m_oper;
        const Expr<T> m_right;

    public:
        Binary(Expr<T> left, Token oper, Expr<T> right);
        T accept(Visitor<T>& visitor) override;
};

template<typename T>
class Grouping : public Expr<T>
{
    private:
        const Expr<T> m_expression;

    public:
        Grouping(Expr<T> expression);
        T accept(Visitor<T>& visitor) override;
};

template<typename T>
class Literal : public Expr<T>
{
    private:
        const T m_value;

    public:
        Literal(T value);
        T accept(Visitor<T>& visitor) override;
};

template<typename T>
class Unary : public Expr<T>
{
    private:
        const Token m_oper;
        const Expr<T> m_right;

    public:
        Unary(Token oper, Expr<T> right);
        T accept(Visitor<T>& visitor) override;
};

#endif