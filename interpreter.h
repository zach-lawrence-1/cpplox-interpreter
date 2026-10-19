#ifndef INTERPRETER
#define INTERPRETER

#include <memory>

#include "stmt.h"
#include "interpreterObject.h"

class Interpreter : public Visitor, public StmtVisitor
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
        void visitExpressionStmt(Expression& expr) override;
        void visitPrintStmt(Print& expr) override;
        void interpret(std::vector<std::unique_ptr<Stmt>>& statements);
};

#endif