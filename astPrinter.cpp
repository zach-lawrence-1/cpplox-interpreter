#include "astPrinter.h"

InterpreterObject AstPrinter::visitBinaryExpr(Binary& expr)
{
    return InterpreterObject(parenthesize(expr.m_oper.getLexeme(), *(expr.m_left), *(expr.m_right)));
}

InterpreterObject AstPrinter::visitGroupingExpr(Grouping& expr)
{
    return InterpreterObject(parenthesize("group", *(expr.m_expression)));
}

InterpreterObject AstPrinter::visitLiteralExpr(Literal& expr)
{
    return expr.m_value;
}

InterpreterObject AstPrinter::visitUnaryExpr(Unary& expr)
{
    return InterpreterObject(parenthesize(expr.m_oper.getLexeme(), *(expr.m_right)));
}

InterpreterObject AstPrinter::visitTernaryExpr(Ternary& expr)
{
    return InterpreterObject(parenthesize("?", ":", *(expr.m_expression), *(expr.m_thenExpr), *(expr.m_elseExpr)));
}

std::string AstPrinter::printExpression(std::unique_ptr<Expr>& expr)
{
    return getObjectValueAsString(expr->accept(*this));
}

std::string AstPrinter::parenthesize(std::string name, Expr& expr)
{
    std::string expression = "";

    expression.append("(");
    expression.append(name);
    expression.append(" ");
    expression.append(getObjectValueAsString(expr.accept(*this)));
    expression.append(")");

    return expression;
}

std::string AstPrinter::parenthesize(std::string name, Expr& leftExpr, Expr& rightExpr)
{
    std::string expression = "";

    expression.append("(");
    expression.append(name);
    expression.append(" ");
    expression.append(getObjectValueAsString(leftExpr.accept(*this)));
    expression.append(" ");
    expression.append(getObjectValueAsString(rightExpr.accept(*this)));
    expression.append(")");

    return expression;
}

std::string AstPrinter::parenthesize(std::string leftName, std::string rightName, Expr& leftExpr, Expr& midExpr, Expr& rightExpr)
{
    std::string expression = "";

    expression.append("(");
    expression.append(leftName);
    expression.append(" ");
    expression.append(getObjectValueAsString(leftExpr.accept(*this)));
    expression.append(" ");
    expression.append(getObjectValueAsString(midExpr.accept(*this)));
    expression.append(" ");
    expression.append(rightName);
    expression.append(" ");
    expression.append(getObjectValueAsString(rightExpr.accept(*this)));
    expression.append(")");

    return expression;
}