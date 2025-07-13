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
    //TODO: make copy operator for Token
    //TODO: figure out how to make this easier to do (maybe use unique pointer or dynamic memory)
    //TODO: get this to work with other kinds of types instead of just std::string

    std::string ttest = "123";
    std::string num = "45.67";
    Literal<std::string> L(&ttest);
    Literal<std::string> R(&num);

    Unary<std::string> u(Token(TOKEN_MINUS, "-", 1), &L);
    Token t(TOKEN_STAR, "*", 1);
    Grouping<std::string> g(&R);
    
    Binary<std::string> expression(&u, t, &g);

    std::cout << AstPrinter<std::string>().printExpression(&expression) << std::endl;
}