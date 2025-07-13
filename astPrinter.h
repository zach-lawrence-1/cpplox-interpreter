#ifndef ASTPRINT
#define ASTPRINT

#include "ast.h"

template<typename T>
class AstPrinter : public Visitor<T>
{
    public:
        AstPrinter() = default;
        std::string visitBinaryExpr(Binary<T>* expr) override;
        std::string visitGroupingExpr(Grouping<T>* expr) override;
        std::string visitLiteralExpr(Literal<T>* expr) override;
        std::string visitUnaryExpr(Unary<T>* expr) override;
        std::string printExpression(Expr<T>* expr);
        std::string parenthesize(std::string name, Expr<T>* leftExpr, Expr<T>* rightExpr);
        std::string parenthesize(std::string name, Expr<T>* Expr);
};


template<typename T>
std::string AstPrinter<T>::visitBinaryExpr(Binary<T>* expr)
{
    return parenthesize(expr->m_oper.getLexeme(), expr->m_left, expr->m_right);
}

template<typename T>
std::string AstPrinter<T>::visitGroupingExpr(Grouping<T>* expr)
{
    return parenthesize("group", expr->m_expression);
}

template<typename T>
std::string AstPrinter<T>::visitLiteralExpr(Literal<T>* expr)
{
    if (expr->m_value == nullptr)
        return "nil";
    return *(expr->m_value);
}

template<typename T>
std::string AstPrinter<T>::visitUnaryExpr(Unary<T>* expr)
{
    return parenthesize(expr->m_oper.getLexeme(), expr->m_right);
}

template<typename T>
std::string AstPrinter<T>::printExpression(Expr<T>* expr)
{
    return expr->accept(this);
}

template<typename T>
std::string AstPrinter<T>::parenthesize(std::string name, Expr<T>* leftExpr, Expr<T>* rightExpr)
{
    std::string expression = "";

    expression.append("(");
    expression.append(name);
    expression.append(" ");
    expression.append(leftExpr->accept(this));
    expression.append(" ");
    expression.append(rightExpr->accept(this));
    expression.append(")");

    return expression;
}

template<typename T>
std::string AstPrinter<T>::parenthesize(std::string name, Expr<T>* Expr)
{
    std::string expression = "";

    expression.append("(");
    expression.append(name);
    expression.append(" ");
    expression.append(Expr->accept(this));
    expression.append(")");

    return expression;
}

#endif