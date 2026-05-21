#include <gtest/gtest.h>
#include "./../scanner.h"

TEST(ScannerHelperFunctions, peek)
{
    Scanner scanner("var x = 10;");
    EXPECT_EQ(scanner.peekChar(), 'v');
    EXPECT_EQ(scanner.peekNextChar(), 'a');
    
    //peek should not change the position the scanner is looking at
    EXPECT_EQ(scanner.peekChar(), 'v');

    while (static_cast<size_t>(scanner.getCurrent()) < scanner.getSource().size() - 1)
    {
        scanner.advance();
    }

    EXPECT_EQ(scanner.peekChar(), ';');
    EXPECT_EQ(scanner.peekNextChar(), '\0');

    scanner.advance();

    EXPECT_EQ(scanner.peekChar(), '\0');
    EXPECT_EQ(scanner.peekNextChar(), '\0');
}

TEST(ScannerHelperFunctions, advance)
{
    Scanner scanner("var x = 10;");

    EXPECT_EQ(scanner.advance(), 'v');
    EXPECT_EQ(scanner.peekChar(), 'a');
}

TEST(ScannerTest, HandlesSimpleInput)
{
    Scanner scanner("var x = (10 + 4);");
    std::vector<Token> tokens = scanner.scanTokens();
    ASSERT_NE(tokens.size(), 0u);

    //should be 10 including the EOF token
    EXPECT_EQ(tokens.size(), 10u);
    //TODO: add more tests
}

