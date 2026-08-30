#ifndef INTERPRETER
#define INTERPRETER

#include <memory>

#include "ast.h"
#include "interpreterObject.h"

class Interpreter : public Visitor
{
    private:
        InterpreterObject evaluate(std::unique_ptr<Expr>& expr);
        bool isTruthy(InterpreterObject& obj);

    public:
        Interpreter() = default;
        InterpreterObject visitBinaryExpr(Binary& expr) override;
        InterpreterObject visitGroupingExpr(Grouping& expr) override;
        InterpreterObject visitLiteralExpr(Literal& expr) override;
        InterpreterObject visitUnaryExpr(Unary& expr) override;
        InterpreterObject visitTernaryExpr(Ternary& expr) override;
        void interpret(std::unique_ptr<Expr>& expr);
};

#endif