#include "scanner.h"
#include "execution.h"

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