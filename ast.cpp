#include "ast.h"

Binary::Binary(Expr& left, Token& oper, Expr& right)
{
    m_left = &left;
    m_oper = &oper;
    m_right = &right;
}

InterpreterObject Binary::accept(Visitor& visitor)
{
    return visitor.visitBinaryExpr(*this);
}

Grouping::Grouping(Expr& expression)
{
    m_expression = &expression;
}

InterpreterObject Grouping::accept(Visitor& visitor)
{
    return visitor.visitGroupingExpr(*this);
}

Literal::Literal(InterpreterObject& value)
{
    m_value = value;
}

InterpreterObject Literal::accept(Visitor& visitor)
{
    return visitor.visitLiteralExpr(*this);
}

Unary::Unary(Token& oper, Expr& right)
{
    m_oper = &oper;
    m_right = &right;
}

InterpreterObject Unary::accept(Visitor& visitor)
{
    return visitor.visitUnaryExpr(*this);
}

