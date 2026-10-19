#include "stmt.h"

Expression::Expression(std::unique_ptr<Expr> expression)
{
    m_expression = std::move(expression);
}

void Expression::accept(StmtVisitor& visitor)
{
    visitor.visitExpressionStmt(*this);
}

Print::Print(std::unique_ptr<Expr> expression)
{
    m_expression = std::move(expression);
}

void Print::accept(StmtVisitor& visitor)
{
    visitor.visitPrintStmt(*this);
}
