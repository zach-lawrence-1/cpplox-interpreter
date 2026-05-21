#include <gtest/gtest.h>
#include "./../parser.h"
#include "./../scanner.h"
#include "./../execution.h"

TEST(ParserTest, ParseSimpleFile)
{
    std::string allFileData = "";
    std::ifstream fileReader("./../hello.cpplox");
    
    //TODO: restructure/refactor this
    if (fileReader.is_open())
    {
        std::string currentLine = "";
        
        while (std::getline(fileReader, currentLine))
        {
            allFileData.append(currentLine + "\n");
        }

        fileReader.close();
        Scanner scanner(allFileData);
        std::vector<Token> tokens = scanner.scanTokens();
        Parser parser(tokens);
        std::unique_ptr<Expr> expression = parser.parse();
        ASSERT_NE(expression, nullptr);
    }
    
    //TODO: add more tests, maybe check size of tokens vec before passing into parser
}