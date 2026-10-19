#include <gtest/gtest.h>
#include "./../astPrinter.h"

namespace {
    std::unique_ptr<Expr> num(double value)
    {
        InterpreterObject obj(value);
        return std::make_unique<Literal>(obj);
    }

    std::unique_ptr<Expr> str(const std::string& value)
    {
        InterpreterObject obj(value);
        return std::make_unique<Literal>(obj);
    }

    std::unique_ptr<Expr> boolean(bool value)
    {
        InterpreterObject obj(value);
        return std::make_unique<Literal>(obj);
    }

    std::unique_ptr<Expr> nil()
    {
        InterpreterObject obj;
        return std::make_unique<Literal>(obj);
    }

    std::unique_ptr<Expr> unary(TokenType type, const std::string& lexeme, std::unique_ptr<Expr> right)
    {
        Token oper(type, lexeme, 1);
        return std::make_unique<Unary>(oper, std::move(right));
    }

    std::unique_ptr<Expr> group(std::unique_ptr<Expr> inner)
    {
        return std::make_unique<Grouping>(std::move(inner));
    }

    std::unique_ptr<Expr> binary(std::unique_ptr<Expr> left, TokenType type,
                                        const std::string& lexeme, std::unique_ptr<Expr> right)
    {
        Token oper(type, lexeme, 1);
        return std::make_unique<Binary>(std::move(left), oper, std::move(right));
    }

    std::unique_ptr<Expr> ternary(std::unique_ptr<Expr> condition, std::unique_ptr<Expr> thenExpr,
                                         std::unique_ptr<Expr> elseExpr)
    {
        return std::make_unique<Ternary>(std::move(condition), std::move(thenExpr), std::move(elseExpr));
    }

    std::string print(std::unique_ptr<Expr>& expr)
    {
        AstPrinter printer;
        return printer.printExpression(expr);
    }
}

TEST(AstPrinterTest, PrintsNumberLiteral)
{
    std::unique_ptr<Expr> expr = num(8);
    EXPECT_EQ(print(expr), "8.000000");
}

TEST(AstPrinterTest, PrintsStringLiteral)
{
    std::unique_ptr<Expr> expr = str("8");
    EXPECT_EQ(print(expr), "8");
}

TEST(AstPrinterTest, PrintsBooleanLiterals)
{
    std::unique_ptr<Expr> yes = boolean(true);
    std::unique_ptr<Expr> no = boolean(false);
    EXPECT_EQ(print(yes), "true");
    EXPECT_EQ(print(no), "false");
}

TEST(AstPrinterTest, PrintsNilLiteral)
{
    std::unique_ptr<Expr> expr = nil();
    EXPECT_EQ(print(expr), "nil");
}

TEST(AstPrinterTest, PrintsEachBinaryOperator)
{
    struct Case { TokenType type; std::string lexeme; };
    std::vector<Case> cases = {
        {TOKEN_PLUS, "+"},         {TOKEN_MINUS, "-"},
        {TOKEN_STAR, "*"},         {TOKEN_SLASH, "/"},
        {TOKEN_EQUAL_EQUAL, "=="}, {TOKEN_NOT_EQUAL, "!="},
        {TOKEN_GREATER, ">"},      {TOKEN_GREATER_EQUAL, ">="},
        {TOKEN_LESS, "<"},         {TOKEN_LESS_EQUAL, "<="},
    };

    for (auto& c : cases)
    {
        std::unique_ptr<Expr> expr = binary(num(8), c.type, c.lexeme, num(7));
        EXPECT_EQ(print(expr), "(" + c.lexeme + " 8.000000 7.000000)");
    }
}

TEST(AstPrinterTest, PrintsStringConcatenation)
{
    std::unique_ptr<Expr> expr = binary(str("test"), TOKEN_PLUS, "+", str("tree"));
    EXPECT_EQ(print(expr), "(+ test tree)");
}

TEST(AstPrinterTest, PrintsCommaOperator)
{
    std::unique_ptr<Expr> expr = binary(num(1), TOKEN_COMMA, ",", num(2));
    EXPECT_EQ(print(expr), "(, 1.000000 2.000000)");
}

TEST(AstPrinterTest, PrintsSimpleNestedBinary)
{
    //8 + (2 * 3)
    std::unique_ptr<Expr> expr = binary(num(8), TOKEN_PLUS, "+", binary(num(2), TOKEN_STAR, "*", num(3)));
    EXPECT_EQ(print(expr), "(+ 8.000000 (* 2.000000 3.000000))");
}

TEST(AstPrinterTest, PrintsUnaryMinus)
{
    std::unique_ptr<Expr> expr = unary(TOKEN_MINUS, "-", num(8));
    EXPECT_EQ(print(expr), "(- 8.000000)");
}

TEST(AstPrinterTest, PrintsUnaryNot)
{
    std::unique_ptr<Expr> notTrue = unary(TOKEN_NOT, "!", boolean(true));
    std::unique_ptr<Expr> notFalse = unary(TOKEN_NOT, "!", boolean(false));
    EXPECT_EQ(print(notTrue), "(! true)");
    EXPECT_EQ(print(notFalse), "(! false)");
}

TEST(AstPrinterTest, PrintsGroupedString)
{
    std::unique_ptr<Expr> expr = group(str("left"));
    EXPECT_EQ(print(expr), "(group left)");
}

TEST(AstPrinterTest, PrintsGroupedOperands)
{
    //(8) != (7)
    std::unique_ptr<Expr> expr = binary(group(num(8)), TOKEN_NOT_EQUAL, "!=", group(num(7)));
    EXPECT_EQ(print(expr), "(!= (group 8.000000) (group 7.000000))");
}

TEST(AstPrinterTest, PrintsTernary)
{
    //5 == 5 ? "same" : "not same"
    std::unique_ptr<Expr> expr = ternary(binary(num(5), TOKEN_EQUAL_EQUAL, "==", num(5)),
                        str("same"),
                        str("not same"));
    EXPECT_EQ(print(expr),
              "(? (== 5.000000 5.000000) same : not same)");
}

TEST(AstPrinterTest, PrintsNestedTernary)
{
    //4 ? (5 ? 4 : 3) : (5 ? 3 : 2)
    std::unique_ptr<Expr> expr = ternary(num(4),
                        ternary(num(5), num(4), num(3)),
                        ternary(num(5), num(3), num(2)));
    EXPECT_EQ(print(expr),
              "(? 4.000000 (? 5.000000 4.000000 : 3.000000) : (? 5.000000 3.000000 : 2.000000))");
}
