#include <gtest/gtest.h>
#include "./../scanner.h"

TEST(ScannerTest, HandlesSimpleInput) {
    Scanner scanner("var x = 10;");
    std::vector<Token> tokens = scanner.scanTokens();
    ASSERT_NE(tokens.size(), 0);
    //TODO: add more tests
}