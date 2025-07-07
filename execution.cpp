#include "execution.h"
#include "scanner.h"

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

void run(const std::string& allFileData)
{
    Scanner scanner(allFileData);
    std::vector<Token> tokens = scanner.scanTokens();

    for (int i = 0; i < int(tokens.size()); i++)
    {
        std::cout << "Lexeme: " << tokens[i].getLexeme() << " type of token: " << tokens[i].getType() << std::endl;
    }
}

void promptCode()
{
    for (;;)
    {
        std::cout << "> ";
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