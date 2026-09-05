#include <gtest/gtest.h>
#include "./../scanner.h"
#include "./../execution.h"

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

void expectTokensMatch(const std::vector<Token>& actual, const std::vector<Token>& expected)
{
    ASSERT_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < actual.size(); i++)
    {
        SCOPED_TRACE("token index " + std::to_string(i));
        EXPECT_EQ(actual[i].getType(), expected[i].getType());
        EXPECT_EQ(actual[i].getLexeme(), expected[i].getLexeme());
        EXPECT_EQ(actual[i].getLine(), expected[i].getLine());
    }
}

TEST(ScannerTest, HandlesSimpleInput)
{
    Scanner scanner("var x = (10 + 4);");
    std::vector<Token> tokens = scanner.scanTokens();
    std::vector<Token> expectedTokens = {
        Token(TOKEN_VAR, "var", 1),
        Token(TOKEN_IDENTIFIER, "x", 1),
        Token(TOKEN_EQUAL, "=", 1),
        Token(TOKEN_LEFT_PAREN, "(", 1),
        Token(TOKEN_NUMBER, "10", 1), 
        Token(TOKEN_PLUS, "+", 1),
        Token(TOKEN_NUMBER, "4", 1),
        Token(TOKEN_RIGHT_PAREN, ")", 1), 
        Token(TOKEN_SEMICOLON, ";", 1),
        Token(TOKEN_EOF, "", 1)
    };

    //should be 10 including the EOF token
    EXPECT_EQ(tokens.size(), 10u);
    
    expectTokensMatch(tokens, expectedTokens);
}

TEST(ScannerTest, HandlesAllSingleTokensCorrectly)
{
    std::string source =
        "( ) { } , . - + ; / * ? "
        "! != = == > >= < <= : "
        "myVar \"a string\" 123.456 "
        "and class else false for fun if nil or print return super this true var while";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
    std::vector<Token> expectedTokens = {
        Token(TOKEN_LEFT_PAREN,    "(",  1),
        Token(TOKEN_RIGHT_PAREN,   ")",  1),
        Token(TOKEN_LEFT_BRACE,    "{",  1),
        Token(TOKEN_RIGHT_BRACE,   "}",  1),
        Token(TOKEN_COMMA,         ",",  1),
        Token(TOKEN_DOT,           ".",  1),
        Token(TOKEN_MINUS,         "-",  1),
        Token(TOKEN_PLUS,          "+",  1),
        Token(TOKEN_SEMICOLON,     ";",  1),
        Token(TOKEN_SLASH,         "/",  1),
        Token(TOKEN_STAR,          "*",  1),
        Token(TOKEN_QUESTION,      "?",  1),

        Token(TOKEN_NOT,           "!",  1),
        Token(TOKEN_NOT_EQUAL,     "!=", 1),
        Token(TOKEN_EQUAL,         "=",  1),
        Token(TOKEN_EQUAL_EQUAL,   "==", 1),
        Token(TOKEN_GREATER,       ">",  1),
        Token(TOKEN_GREATER_EQUAL, ">=", 1),
        Token(TOKEN_LESS,          "<",  1),
        Token(TOKEN_LESS_EQUAL,    "<=", 1),
        Token(TOKEN_COLON,         ":",  1),

        Token(TOKEN_IDENTIFIER,    "myVar",       1),
        Token(TOKEN_STRING,        "a string",1),
        Token(TOKEN_NUMBER,        "123.456",     1),

        Token(TOKEN_AND,    "and",    1),
        Token(TOKEN_CLASS,  "class",  1),
        Token(TOKEN_ELSE,   "else",   1),
        Token(TOKEN_FALSE,  "false",  1),
        Token(TOKEN_FOR,    "for",    1),
        Token(TOKEN_FUN,    "fun",    1),
        Token(TOKEN_IF,     "if",     1),
        Token(TOKEN_NIL,    "nil",    1),
        Token(TOKEN_OR,     "or",     1),
        Token(TOKEN_PRINT,  "print",  1),
        Token(TOKEN_RETURN, "return", 1),
        Token(TOKEN_SUPER,  "super",  1),
        Token(TOKEN_THIS,   "this",   1),
        Token(TOKEN_TRUE,   "true",   1),
        Token(TOKEN_VAR,    "var",    1),
        Token(TOKEN_WHILE,  "while",  1),

        Token(TOKEN_EOF, "", 1)
    };

    expectTokensMatch(tokens, expectedTokens);
}

TEST(ScannerTest, IdentifierThatStartsWithKeywordIsNotTreatedAsKeyword)
{
    //checks against misclassification of variable as keyword
    Scanner scanner("forEach andAlso printer ifAllElse elseAllIf variable");
    std::vector<Token> tokens = scanner.scanTokens();
    std::vector<Token> expectedTokens = {
        Token(TOKEN_IDENTIFIER, "forEach", 1),
        Token(TOKEN_IDENTIFIER, "andAlso", 1),
        Token(TOKEN_IDENTIFIER, "printer", 1),
        Token(TOKEN_IDENTIFIER, "ifAllElse", 1),
        Token(TOKEN_IDENTIFIER, "elseAllIf", 1),
        Token(TOKEN_IDENTIFIER, "variable", 1),
        Token(TOKEN_EOF, "", 1)
    };

    expectTokensMatch(tokens, expectedTokens);
}

TEST(ScannerTest, SkipsCommentsAndWhitespaceAndTracksNewLines)
{
    std::string source =
        "var a = 1; // this is a comment\n"
        "var b = 2;\n"
        "\n"
        "var c = 3;";
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
    std::vector<Token> expectedTokens = {
        Token(TOKEN_VAR, "var", 1),
        Token(TOKEN_IDENTIFIER, "a", 1),
        Token(TOKEN_EQUAL, "=", 1),
        Token(TOKEN_NUMBER, "1", 1),
        Token(TOKEN_SEMICOLON, ";", 1),
        Token(TOKEN_VAR, "var", 2),
        Token(TOKEN_IDENTIFIER, "b", 2),
        Token(TOKEN_EQUAL, "=", 2),
        Token(TOKEN_NUMBER, "2", 2),
        Token(TOKEN_SEMICOLON, ";", 2),
        Token(TOKEN_VAR, "var", 4),
        Token(TOKEN_IDENTIFIER, "c", 4),
        Token(TOKEN_EQUAL, "=", 4),
        Token(TOKEN_NUMBER, "3", 4),
        Token(TOKEN_SEMICOLON, ";", 4),
        Token(TOKEN_EOF, "", 4)
    };

    expectTokensMatch(tokens, expectedTokens);
}

TEST(ScannerTest, SingleLineCommentIsIgnored)
{
    Scanner scanner("var a = 1; // rest of the line is ignored\nvar b = 2;");
    std::vector<Token> tokens = scanner.scanTokens();
    std::vector<Token> expectedTokens = {
        Token(TOKEN_VAR, "var", 1),
        Token(TOKEN_IDENTIFIER, "a", 1),
        Token(TOKEN_EQUAL, "=", 1),
        Token(TOKEN_NUMBER, "1", 1),
        Token(TOKEN_SEMICOLON, ";", 1),
        Token(TOKEN_VAR, "var", 2),
        Token(TOKEN_IDENTIFIER, "b", 2),
        Token(TOKEN_EQUAL, "=", 2),
        Token(TOKEN_NUMBER, "2", 2),
        Token(TOKEN_SEMICOLON, ";", 2),
        Token(TOKEN_EOF, "", 2)
    };

    expectTokensMatch(tokens, expectedTokens);
}

TEST(ScannerTest, MultiLineCommentIsIgnoredAndTracksNewLines)
{
    Scanner scanner("var a = 1; /* this\nspans\nlines */ var b = 2;");
    std::vector<Token> tokens = scanner.scanTokens();
    std::vector<Token> expectedTokens = {
        Token(TOKEN_VAR, "var", 1),
        Token(TOKEN_IDENTIFIER, "a", 1),
        Token(TOKEN_EQUAL, "=", 1),
        Token(TOKEN_NUMBER, "1", 1),
        Token(TOKEN_SEMICOLON, ";", 1),
        Token(TOKEN_VAR, "var", 1),
        Token(TOKEN_IDENTIFIER, "b", 1),
        Token(TOKEN_EQUAL, "=", 1),
        Token(TOKEN_NUMBER, "2", 1),
        Token(TOKEN_SEMICOLON, ";", 1),
        Token(TOKEN_EOF, "", 1)
    };

    expectTokensMatch(tokens, expectedTokens);
}

//fixture that resets errorExists on start and end of each test automatically
class ScannerErrorTest : public ::testing::Test
{
    protected:
        void SetUp() override { errorExists = false; }
        void TearDown() override { errorExists = false; }
};

void expectErrorContains(const std::string& output, const std::string& expectedSubstring)
{
    EXPECT_NE(output.find(expectedSubstring), std::string::npos)
        << "Expected error output to contain: \"" << expectedSubstring << "\"\n"
        << "Actual captured output was:        \"" << output << "\"";
}

TEST(ScannerTest, NestedMultiLineCommentIsIgnored)
{
    errorExists = false;
    Scanner scanner("/* outer /* inner */ still outer */ var x = 1;");
    std::vector<Token> tokens = scanner.scanTokens();
    std::vector<Token> expectedTokens = {
        Token(TOKEN_VAR, "var", 1),
        Token(TOKEN_IDENTIFIER, "x", 1),
        Token(TOKEN_EQUAL, "=", 1),
        Token(TOKEN_NUMBER, "1", 1),
        Token(TOKEN_SEMICOLON, ";", 1),
        Token(TOKEN_EOF, "", 1)
    };

    EXPECT_FALSE(errorExists);
    expectTokensMatch(tokens, expectedTokens);
}

TEST_F(ScannerErrorTest, UnterminatedMultiLineCommentReportsError)
{
    testing::internal::CaptureStderr();
    Scanner scanner("/* multiline never closed");
    scanner.scanTokens();

    EXPECT_TRUE(errorExists);
    std::string errorOutput = testing::internal::GetCapturedStderr();
    expectErrorContains(errorOutput, "Missing multiline comment closure");
}

TEST_F(ScannerErrorTest, UnterminatedNestedMultiLineCommentReportsError)
{
    testing::internal::CaptureStderr();
    Scanner scanner("/* outer /* inner comment never closes */");
    scanner.scanTokens();

    EXPECT_TRUE(errorExists);
    std::string errorOutput = testing::internal::GetCapturedStderr();
    expectErrorContains(errorOutput, "Missing multiline comment closure");
}

TEST_F(ScannerErrorTest, UnterminatedStringReportsError)
{
    testing::internal::CaptureStderr();
    Scanner scanner("var x = \"unterminated");
    scanner.scanTokens();
 
    EXPECT_TRUE(errorExists);
    std::string errorOutput = testing::internal::GetCapturedStderr();
    expectErrorContains(errorOutput, "Unterminated string");
}

TEST_F(ScannerErrorTest, UnexpectedCharacterReportsError)
{
    testing::internal::CaptureStderr();
    Scanner scanner("var x = @;");
    scanner.scanTokens();
    std::string output = testing::internal::GetCapturedStderr();
 
    EXPECT_TRUE(errorExists);
    expectErrorContains(output, "Unexpected character");
}
 
TEST_F(ScannerErrorTest, WellFormedInputDoesNotReportError)
{
    testing::internal::CaptureStderr();
    Scanner scanner("var x = 10;");
    scanner.scanTokens();
    std::string output = testing::internal::GetCapturedStderr();
 
    EXPECT_FALSE(errorExists);
    EXPECT_TRUE(output.empty());
}

TEST_F(ScannerErrorTest, ErrorReportsHasCorrectLineNumber)
{
    testing::internal::CaptureStderr();
    Scanner scanner("var a = 1;\nvar b = @;");
    scanner.scanTokens();
    std::string output = testing::internal::GetCapturedStderr();
 
    EXPECT_TRUE(errorExists);
    expectErrorContains(output, "Line 2");
}