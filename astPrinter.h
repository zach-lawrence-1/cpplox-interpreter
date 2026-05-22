#ifndef ASTPRINT
#define ASTPRINT

#include <memory>

#include "ast.h"
#include "interpreterObject.h"

class AstPrinter : public Visitor
{
    public:
        AstPrinter() = default;
        InterpreterObject visitBinaryExpr(Binary& expr) override;
        InterpreterObject visitGroupingExpr(Grouping& expr) override;
        InterpreterObject visitLiteralExpr(Literal& expr) override;
        InterpreterObject visitUnaryExpr(Unary& expr) override;
        InterpreterObject visitTernaryExpr(Ternary& expr) override;
        std::string printExpression(std::unique_ptr<Expr>& expr);
        std::string parenthesize(std::string name, Expr& Expr);
        std::string parenthesize(std::string name, Expr& leftExpr, Expr& rightExpr);
        std::string parenthesize(std::string leftName, std::string rightName, Expr& leftExpr, Expr& midExpr, Expr& rightExpr);
};

#endif