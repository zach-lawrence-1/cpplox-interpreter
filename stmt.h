#ifndef STMT
#define STMT

#include <memory>
#include "scanner.h"
#include "interpreterObject.h"
#include "ast.h"

class Expression;
class Print;

class StmtVisitor
{
    public:
        virtual void visitExpressionStmt(Expression& expr) = 0;
        virtual void visitPrintStmt(Print& expr) = 0;
};

class Stmt
{
    public:
        virtual void accept(StmtVisitor& visitor) = 0;
        virtual ~Stmt() = default;
};

class Expression : public Stmt
{
    public:
        std::unique_ptr<Expr> m_expression;

    public:
        Expression(std::unique_ptr<Expr> expression);
        void accept(StmtVisitor& visitor) override;
};

class Print : public Stmt
{
    public:
        std::unique_ptr<Expr> m_expression;

    public:
        Print(std::unique_ptr<Expr> expression);
        void accept(StmtVisitor& visitor) override;
};

#endif