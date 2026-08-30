#include "interpreter.h"
#include "execution.h"

InterpreterObject Interpreter::evaluate(std::unique_ptr<Expr>& expr)
{
    return expr->accept(*this);
}

bool Interpreter::isTruthy(InterpreterObject& obj)
{
    //use nil as false, and anything else that is not a bool as true
    if (obj.getType() == NIL)
        return false;
    else if (obj.getType() == BOOL)
        return obj.getBoolValue();
    return true;
}

InterpreterObject Interpreter::visitBinaryExpr(Binary& expr)
{
    InterpreterObject left = evaluate(expr.m_left);
    InterpreterObject right = evaluate(expr.m_right);
    Token oper = expr.m_oper;

    if (left.getType() != right.getType())
    {
        runtimeError(oper, "Operand types do not match.");
        return InterpreterObject();
    }

    switch (oper.getType())
    {
        case TOKEN_MINUS:
            if (left.getType() == NUMBER)
                return InterpreterObject(left.getNumberValue() - right.getNumberValue());
            
            runtimeError(oper, "Operands must be numbers.");
            break;
        case TOKEN_STAR:
            if (left.getType() == NUMBER)
                return InterpreterObject(left.getNumberValue() * right.getNumberValue());
            
            runtimeError(oper, "Operands must be numbers.");
            break;
        case TOKEN_SLASH:
            if (left.getType() == NUMBER)
                return InterpreterObject(left.getNumberValue() / right.getNumberValue());
            
            runtimeError(oper, "Operands must be numbers.");
            break;
        case TOKEN_PLUS:
            if (left.getType() == NUMBER)
                return InterpreterObject(left.getNumberValue() + right.getNumberValue());
            else if (left.getType() == STRING)
                return InterpreterObject(left.getStringValue() + right.getStringValue());
            
            runtimeError(oper, "Operands must be either numbers or strings.");
            break;
        case TOKEN_GREATER_EQUAL:
            if (left.getType() == NUMBER)
                return InterpreterObject(left.getNumberValue() >= right.getNumberValue());
            break;
        case TOKEN_LESS_EQUAL:
            if (left.getType() == NUMBER)
                return InterpreterObject(left.getNumberValue() <= right.getNumberValue());
            break;
        case TOKEN_GREATER:
            if (left.getType() == NUMBER)
                return InterpreterObject(left.getNumberValue() > right.getNumberValue());
            break;
        case TOKEN_LESS:
            if (left.getType() == NUMBER)
                return InterpreterObject(left.getNumberValue() < right.getNumberValue());
            break;
        case TOKEN_EQUAL_EQUAL:
            return InterpreterObject((left == right));
        case TOKEN_NOT_EQUAL:
            return InterpreterObject(!(left == right));
        default:
            break;
    }

    return InterpreterObject();
}

InterpreterObject Interpreter::visitGroupingExpr(Grouping& expr)
{
    return evaluate(expr.m_expression);
}

InterpreterObject Interpreter::visitLiteralExpr(Literal& expr)
{
    return InterpreterObject(expr.m_value);
}

InterpreterObject Interpreter::visitUnaryExpr(Unary& expr)
{
    Token oper = expr.m_oper;
    InterpreterObject literal = evaluate(expr.m_right);

    switch (oper.getType())
    {
        case TOKEN_MINUS:
            if (literal.getType() != NUMBER)
            {
                runtimeError(oper, "Operand must be a number.");
                break;
            }

            return InterpreterObject(-literal.getNumberValue());
        case TOKEN_NOT:
            return InterpreterObject(!isTruthy(literal));
        default:
            break;
    }

    return InterpreterObject();
}

InterpreterObject Interpreter::visitTernaryExpr(Ternary& expr)
{
    InterpreterObject elseExpr = evaluate(expr.m_elseExpr);
    InterpreterObject expression = evaluate(expr.m_expression);
    InterpreterObject thenExpr = evaluate(expr.m_thenExpr);
    
    return InterpreterObject();
}

void Interpreter::interpret(std::unique_ptr<Expr>& expr)
{
    InterpreterObject result = evaluate(expr);
    
    if (!runtimeErrorExists)
    {
        std::string ostr = getObjectValueAsString(result);
        std::cout << ostr << std::endl;
    }
}