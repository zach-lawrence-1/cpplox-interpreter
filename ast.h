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
        virtual T visitBinaryExpr(Binary<T>* expr) = 0;
        virtual T visitGroupingExpr(Grouping<T>* expr) = 0;
        virtual T visitLiteralExpr(Literal<T>* expr) = 0;
        virtual T visitUnaryExpr(Unary<T>* expr) = 0;
};

template<typename T>
class Expr
{
    public:
        virtual T accept(Visitor<T>* visitor) = 0;
};

template<typename T>
class Binary : public Expr<T>
{
    public:
        Expr<T>* m_left;
        Token m_oper;
        Expr<T>* m_right;

    public:
        Binary(Expr<T>* left, Token oper, Expr<T>* right);
        T accept(Visitor<T>* visitor) override;
};

template<typename T>
class Grouping : public Expr<T>
{
    public:
        Expr<T>* m_expression;

    public:
        Grouping(Expr<T>* expression);
        T accept(Visitor<T>* visitor) override;
};

template<typename T>
class Literal : public Expr<T>
{
    public:
        T* m_value;

    public:
        Literal(T* value);
        T accept(Visitor<T>* visitor) override;
};

template<typename T>
class Unary : public Expr<T>
{
    public:
        Token m_oper;
        Expr<T>* m_right;

    public:
        Unary(const Token& oper, Expr<T>* right);
        T accept(Visitor<T>* visitor) override;
};

template<typename T>
Binary<T>::Binary(Expr<T>* left, Token oper, Expr<T>* right)
{
    m_left = left;
    m_oper = oper;
    m_right = right;
}

template<typename T>
T Binary<T>::accept(Visitor<T>* visitor)
{
    return visitor->visitBinaryExpr(this);
}

template<typename T>
Grouping<T>::Grouping(Expr<T>* expression)
{
    m_expression = expression;
}

template<typename T>
T Grouping<T>::accept(Visitor<T>* visitor)
{
    return visitor->visitGroupingExpr(this);
}

template<typename T>
Literal<T>::Literal(T* value)
{
    m_value = value;
}

template<typename T>
T Literal<T>::accept(Visitor<T>* visitor)
{
    return visitor->visitLiteralExpr(this);
}

template<typename T>
Unary<T>::Unary(const Token& oper, Expr<T>* right)
{
    m_oper = oper;
    m_right = right;
}

template<typename T>
T Unary<T>::accept(Visitor<T>* visitor)
{
    return visitor->visitUnaryExpr(this);
}

#endif