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
    //TODO: rewrite to not use templates or something
    //TODO: figure out how to make this easier to do (maybe use unique pointer or dynamic memory)
    //TODO: get this to work with other kinds of types instead of just std::string

    Literal<int> L(123);
    Literal<float> R(45.67f);

    Unary<int> u(Token(TOKEN_MINUS, "-", 1), L);
    Token t(TOKEN_STAR, "*", 1);
    Grouping<float> g(R);
    
    Binary<std::string, int, float> expression(u, t, g);

    std::cout << AstPrinter<std::string, int, float>().printExpression(expression) << std::endl;
}