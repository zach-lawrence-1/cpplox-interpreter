#ifndef INTERPRETER
#define INTERPRETER

#include <memory>

#include "stmt.h"
#include "interpreterObject.h"
#include "environment.h"

class Interpreter : public Visitor, public StmtVisitor
{
    private:
        Environment environment;

        InterpreterObject evaluate(std::unique_ptr<Expr>& expr);
        bool isTruthy(InterpreterObject& obj);

    public:
        Interpreter() = default;
        InterpreterObject visitBinaryExpr(Binary& expr) override;
        InterpreterObject visitGroupingExpr(Grouping& expr) override;
        InterpreterObject visitLiteralExpr(Literal& expr) override;
        InterpreterObject visitUnaryExpr(Unary& expr) override;
        InterpreterObject visitTernaryExpr(Ternary& expr) override;
        InterpreterObject visitVariableExpr(Variable& expr) override;
        void visitExpressionStmt(Expression& stmt) override;
        void visitPrintStmt(Print& stmt) override;
        void visitVarStmt(Var& stmt) override;
        void interpret(std::vector<std::unique_ptr<Stmt>>& statements);
};

#endif