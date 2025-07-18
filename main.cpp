#include <memory>

#include "scanner.h"
#include "execution.h"
#include "ast.h"
#include "astPrinter.h"

/*
int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        //allow user to write lox code
        promptCode();
    }
    if (argc == 2)
    {
        //run user specified file
        runFile(argv[1]);
    }
    else if (argc > 2)
    {
        std::cout << "\033[31mtoo many arguments\033[0m";
        exit(-1);
    }
}
    */


//NOTE: temporary main function for testing ast implementation
int main()
{
    std::string test = "123";
    InterpreterObject firstObject(test);
    InterpreterObject secondObject(1);
    Token minus(TOKEN_MINUS, "-", 1);
    Token multiply(TOKEN_STAR, "*", 1);

    Literal L(firstObject);
    Literal R(secondObject);
    Unary u(minus, L);
    Grouping g(R);
    Binary expression(u, multiply, g);
    Binary expression2(expression, multiply, g);

    std::cout << AstPrinter().printExpression(expression2) << std::endl;
}