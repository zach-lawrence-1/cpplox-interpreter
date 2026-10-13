#include "stmt.h"

Expression::Expression(std::unique_ptr<Expr> expression)
{
    m_expression = std::move(expression);
}

InterpreterObject Expression::accept(StmtVisitor& visitor)
{
    return visitor.visitExpressionStmt(*this);
}

Print::Print(std::unique_ptr<Expr> expression)
{
    m_expression = std::move(expression);
}

InterpreterObject Print::accept(StmtVisitor& visitor)
{
    return visitor.visitPrintStmt(*this);
}
