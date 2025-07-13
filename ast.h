#ifndef AST
#define AST

#include "scanner.h"

template<typename T, typename U, typename W>
class Binary;

template<typename T>
class Grouping;

template<typename T>
class Literal;

template<typename T>
class Unary;

template<typename T, typename U, typename W>
class Visitor
{
    public:
        virtual T visitBinaryExpr(Binary<T, U, W>& expr) = 0;
        virtual T visitGroupingExpr(Grouping<T>& expr) = 0;
        virtual T visitLiteralExpr(Literal<T>& expr) = 0;
        virtual T visitUnaryExpr(Unary<T>& expr) = 0;
};

template<typename T, typename U, typename W>
class Expr
{
    public:
        virtual T accept(Visitor<T, U, W>& visitor) = 0;
};

template<typename T, typename U, typename W>
class Binary : public Expr<T, U, W>
{
    public:
        Expr<U, U, U>* m_left;
        Token m_oper;
        Expr<W, W, W>* m_right;

    public:
        Binary(Expr<U, U, U>& left, Token oper, Expr<W, W, W>& right);
        T accept(Visitor<T, U, W>& visitor) override;
};

template<typename T>
class Grouping : public Expr<T, T, T>
{
    public:
        Expr<T, T, T>* m_expression;

    public:
        Grouping(Expr<T, T, T>& expression);
        T accept(Visitor<T, T, T>& visitor) override;
};

template<typename T>
class Literal : public Expr<T, T, T>
{
    public:
        T m_value;

    public:
        Literal(T value);
        T accept(Visitor<T, T, T>& visitor) override;
};

template<typename T>
class Unary : public Expr<T, T, T>
{
    public:
        Token m_oper;
        Expr<T, T, T>* m_right;

    public:
        Unary(const Token& oper, Expr<T, T, T>& right);
        T accept(Visitor<T, T, T>& visitor) override;
};


template<typename T, typename U, typename W>
Binary<T, U, W>::Binary(Expr<U, U, U>& left, Token oper, Expr<W, W, W>& right)
{
    m_left = &left;
    m_oper = oper;
    m_right = &right;
}


template<typename T, typename U, typename W>
T Binary<T, U, W>::accept(Visitor<T, U, W>& visitor)
{
    return visitor.visitBinaryExpr(*this);
}

template<typename T>
Grouping<T>::Grouping(Expr<T, T, T>& expression)
{
    m_expression = &expression;
}

template<typename T>
T Grouping<T>::accept(Visitor<T, T, T>& visitor)
{
    return visitor.visitGroupingExpr(*this);
}

template<typename T>
Literal<T>::Literal(T value)
{
    m_value = value;
}

template<typename T>
T Literal<T>::accept(Visitor<T, T, T>& visitor)
{
    return visitor.visitLiteralExpr(*this);
}

template<typename T>
Unary<T>::Unary(const Token& oper, Expr<T, T, T>& right)
{
    m_oper = oper;
    m_right = &right;
}

template<typename T>
T Unary<T>::accept(Visitor<T, T, T>& visitor)
{
    return visitor.visitUnaryExpr(*this);
}

#endif