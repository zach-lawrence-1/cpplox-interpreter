#include "ast.h"

Binary::Binary(std::unique_ptr<Expr> left, Token& oper, std::unique_ptr<Expr> right)
{
    m_left = std::move(left);
    m_oper = oper;
    m_right = std::move(right);
}

InterpreterObject Binary::accept(Visitor& visitor)
{
    return visitor.visitBinaryExpr(*this);
}

Grouping::Grouping(std::unique_ptr<Expr> expression)
{
    m_expression = std::move(expression);
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

Unary::Unary(Token& oper, std::unique_ptr<Expr> right)
{
    m_oper = oper;
    m_right = std::move(right);
}

InterpreterObject Unary::accept(Visitor& visitor)
{
    return visitor.visitUnaryExpr(*this);
}

