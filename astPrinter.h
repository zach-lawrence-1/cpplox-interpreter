#ifndef ASTPRINT
#define ASTPRINT

#include "ast.h"

template<typename T, typename U, typename W>
class AstPrinter : public Visitor<T, U, W>
{
    public:
        AstPrinter() = default;
        T visitBinaryExpr(Binary<T, U, W>& expr) override;
        T visitGroupingExpr(Grouping<T>& expr) override;
        T visitLiteralExpr(Literal<T>& expr) override;
        T visitUnaryExpr(Unary<T>& expr) override;
        std::string printExpression(Expr<T, U, W>& expr);
        std::string parenthesize(std::string name, Expr<U, U, U>& leftExpr, Expr<W, W, W>& rightExpr);
        std::string parenthesize(std::string name, Expr<T, T, T>& Expr);
};


template<typename T, typename U, typename W>
T AstPrinter<T, U, W>::visitBinaryExpr(Binary<T, U, W>& expr)
{
    return parenthesize(expr.m_oper.getLexeme(), *expr.m_left, *expr.m_right);
}

template<typename T, typename U, typename W>
T AstPrinter<T, U, W>::visitGroupingExpr(Grouping<T>& expr)
{
    return parenthesize("group", *expr.m_expression);
}

template<typename T, typename U, typename W>
T AstPrinter<T, U, W>::visitLiteralExpr(Literal<T>& expr)
{
    if (&expr.m_value == nullptr)
        return "nil";
    return expr.m_value;
}

template<typename T, typename U, typename W>
T AstPrinter<T, U, W>::visitUnaryExpr(Unary<T>& expr)
{
    return parenthesize(expr.m_oper.getLexeme(), *expr.m_right);
}

template<typename T, typename U, typename W>
std::string AstPrinter<T, U, W>::printExpression(Expr<T, U, W>& expr)
{
    return expr.accept(*this);
}


template<typename T, typename U, typename W>
std::string AstPrinter<T, U, W>::parenthesize(std::string name, Expr<U, U, U>& leftExpr, Expr<W, W, W>& rightExpr)
{
    std::string expression = "";

    expression.append("(");
    expression.append(name);
    expression.append(" ");
    expression.append(leftExpr.accept(*this));
    expression.append(" ");
    expression.append(rightExpr.accept(*this));
    expression.append(")");

    return expression;
}

template<typename T, typename U, typename W>
std::string AstPrinter<T, U, W>::parenthesize(std::string name, Expr<T, T, T>& Expr)
{
    std::string expression = "";

    expression.append("(");
    expression.append(name);
    expression.append(" ");
    expression.append(Expr.accept(*this));
    expression.append(")");

    return expression;
}

#endif