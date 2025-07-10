#include "astGOAL.h"

template<typename T>
Binary<T>::Binary(Expr<T> left, Token oper, Expr<T> right)
{
    m_left = left;
    m_oper = oper;
    m_right = right;
}

template<typename T>
T Binary<T>::accept(Visitor<T>& visitor)
{
    return visitor.visitBinaryExpr(this);
}

template<typename T>
Grouping<T>::Grouping(Expr<T> expression)
{
    m_expression = expression;
}

template<typename T>
T Grouping<T>::accept(Visitor<T>& visitor)
{
    return visitor.visitGroupingExpr(this);
}

template<typename T>
Literal<T>::Literal(T value)
{
    m_value = value;
}

template<typename T>
T Literal<T>::accept(Visitor<T>& visitor)
{
    return visitor.visitLiteralExpr(this);
}

template<typename T>
Unary<T>::Unary(Token oper, Expr<T> right)
{
    m_oper = oper;
    m_right = right;
}

template<typename T>
T Unary<T>::accept(Visitor<T>& visitor)
{
    return visitor.visitUnaryExpr(this);
}
