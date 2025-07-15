#include "astPrinter.h"

std::string getObjectValueAsString(InterpreterObject obj)
{
    switch(obj.getType())
    {
        case STRING:
            return obj.getStringValue();
        case BOOL:
        {
            if (obj.getBoolValue())
                return "true";
            return "false";
        }
        case NUMBER:
            return std::to_string(obj.getNumberValue());
        case NIL:
            return "nil";
    }

    return "";
}

InterpreterObject AstPrinter::visitBinaryExpr(Binary& expr)
{
    return InterpreterObject(parenthesize(expr.m_oper.getLexeme(), *expr.m_left, *expr.m_right));
}

InterpreterObject AstPrinter::visitGroupingExpr(Grouping& expr)
{
    return InterpreterObject(parenthesize("group", *expr.m_expression));
}

InterpreterObject AstPrinter::visitLiteralExpr(Literal& expr)
{
    return expr.m_value;
}

InterpreterObject AstPrinter::visitUnaryExpr(Unary& expr)
{
    return InterpreterObject(parenthesize(expr.m_oper.getLexeme(), *expr.m_right));
}

std::string AstPrinter::printExpression(Expr& expr)
{
    return expr.accept(*this).getStringValue();
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
