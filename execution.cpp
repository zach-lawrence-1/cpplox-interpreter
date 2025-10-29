#include "execution.h"
#include "scanner.h"
#include "astPrinter.h"
#include "parser.h"

bool errorExists = false;

//TODO: add this capability
/*Error: Unexpected "," in argument list.

    15 | function(first, second,);
                               ^-- Here.*/
void report(const int& line, const std::string& where, const std::string& message)
{
    std::cerr << "\033[31m[Line " << line << "] Error" << where << ": " + message << "\033[0m" << std::endl;
    errorExists = true;
}

void error(const int& line, const std::string& message)
{
    report(line, "", message);
}

void error(Token token, const std::string& message)
{
    if (token.getType() == TOKEN_EOF)
        report(token.getLine(), " at end", message);
    else
        report(token.getLine(), " at '" + token.getLexeme() + "'", message);
}

void run(const std::string& allFileData)
{
    Scanner scanner(allFileData);
    std::vector<Token> tokens = scanner.scanTokens();

    std::cout << "tokens: " << tokens.size() << std::endl;

    Parser parser(tokens);
    std::unique_ptr<Expr> expression = parser.parse();

    if (errorExists)
        return;
    
    AstPrinter printer;

    if (expression == nullptr)
    {
        std::cout << "expression could not be constructed";
        return;
    }
    
    std::cout << printer.printExpression(expression);

    //for (Token token : tokens)
    //{
    //    std::cout << "Lexeme: " << token.getLexeme() << " type of token: " << token.getType() << std::endl;
    //}
}

void promptCode()
{
    for (;;)
    {
        std::cout << std::endl << "> ";
        std::string lineInput = "";
        std::getline(std::cin, lineInput);

        if (lineInput.empty())
            return;
        
        run(lineInput);
        errorExists = false;
    }
}

void runFile(const std::string& file)
{
    std::ifstream fileReader(file);
    
    if (fileReader.is_open())
    {
        std::string allFileData = "";
        std::string currentLine = "";
        
        while (std::getline(fileReader, currentLine))
        {
            allFileData.append(currentLine + "\n");
        }

        fileReader.close();
        run(allFileData);
        
        if (errorExists)
        {
            exit(-1);
        }
    }
    else
    {
        std::cerr << "\033[31mFile " << file << " Could not be opened\033[0m";
        exit(-1);
    }
}