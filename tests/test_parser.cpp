#include <gtest/gtest.h>
#include "./../parser.h"
#include "./../scanner.h"
#include "./../execution.h"

TEST(ParserTest, ParseSimpleExpressions)
{
    std::string allFileData = "9 == 9;"
                              "(\"hello\" + \"world\");"
                              "(9 + 7) * 2 - 1 == (16 - 0) + (9 * 6) / 2;"
                              "print \"hello\" + \" world\";"
                              "print true;";
    
    Scanner scanner(allFileData);
    std::vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();
    ASSERT_EQ(statements.size(), size_t(5));
    //TODO: test structure of the AST generated, and add more tests
}