#include <gtest/gtest.h>
#include <monkey/lexer/lexer.h>

#include <ranges>

namespace monkey::lexer {

static_assert(std::forward_iterator<Lexer::Iterator>);
static_assert(std::ranges::forward_range<Lexer>);
static_assert(std::ranges::forward_range<Lexer &>);
static_assert(std::ranges::forward_range<const Lexer>);
static_assert(std::ranges::forward_range<const Lexer &>);

TEST(MonkeyLexerTest, SingleCharTokens) {
  auto lexer = Lexer("=+(){},;");
  auto expected = std::vector<Token>{
      Token(TokenType::kAssign, "="),    Token(TokenType::kPlus, "+"),
      Token(TokenType::kLeftParen, "("), Token(TokenType::kRightParen, ")"),
      Token(TokenType::kLeftBrace, "{"), Token(TokenType::kRightBrace, "}"),
      Token(TokenType::kComma, ","),     Token(TokenType::kSemicolon, ";"),
  };
  ASSERT_TRUE(std::ranges::equal(lexer, expected));
}

TEST(MonkeyLexerTest, Keywords) {
  auto lexer = Lexer(
      "let five = 5;"
      "let ten = 10;"
      "let add = fn(x, y) {"
      "  x + y;"
      "};"
      "let result = add(five, ten);");
  auto expected = std::vector<Token>{
      Token(TokenType::kLet, "let"),
      Token(TokenType::kIdentifer, "five"),
      Token(TokenType::kAssign, "="),
      Token(TokenType::kInteger, "5"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kLet, "let"),
      Token(TokenType::kIdentifer, "ten"),
      Token(TokenType::kAssign, "="),
      Token(TokenType::kInteger, "10"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kLet, "let"),
      Token(TokenType::kIdentifer, "add"),
      Token(TokenType::kAssign, "="),
      Token(TokenType::kFunction, "fn"),
      Token(TokenType::kLeftParen, "("),
      Token(TokenType::kIdentifer, "x"),
      Token(TokenType::kComma, ","),
      Token(TokenType::kIdentifer, "y"),
      Token(TokenType::kRightParen, ")"),
      Token(TokenType::kLeftBrace, "{"),
      Token(TokenType::kIdentifer, "x"),
      Token(TokenType::kPlus, "+"),
      Token(TokenType::kIdentifer, "y"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kRightBrace, "}"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kLet, "let"),
      Token(TokenType::kIdentifer, "result"),
      Token(TokenType::kAssign, "="),
      Token(TokenType::kIdentifer, "add"),
      Token(TokenType::kLeftParen, "("),
      Token(TokenType::kIdentifer, "five"),
      Token(TokenType::kComma, ","),
      Token(TokenType::kIdentifer, "ten"),
      Token(TokenType::kRightParen, ")"),
      Token(TokenType::kSemicolon, ";"),
  };
  ASSERT_TRUE(std::ranges::equal(lexer, expected));
}

TEST(MonkeyLexerTest, Expressions) {
  auto lexer = Lexer(
      "!-/*5;"
      "5 < 10 > 5;"
      "if (5 < 10) {"
      "	return true;"
      "} else {"
      "	return false;"
      "}"
      "10 == 10;"
      "10 != 9;"
      "\"foobar\""
      "\"foo bar\""
      "[1, 2];"
      "{\"foo\": \"bar\"}"
      "macro(x, y) { x + y; };");
  auto expected = std::vector<Token>{
      Token(TokenType::kBang, "!"),
      Token(TokenType::kMinus, "-"),
      Token(TokenType::kSlash, "/"),
      Token(TokenType::kAsterisk, "*"),
      Token(TokenType::kInteger, "5"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kInteger, "5"),
      Token(TokenType::kLessThan, "<"),
      Token(TokenType::kInteger, "10"),
      Token(TokenType::kGreaterThan, ">"),
      Token(TokenType::kInteger, "5"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kIf, "if"),
      Token(TokenType::kLeftParen, "("),
      Token(TokenType::kInteger, "5"),
      Token(TokenType::kLessThan, "<"),
      Token(TokenType::kInteger, "10"),
      Token(TokenType::kRightParen, ")"),
      Token(TokenType::kLeftBrace, "{"),
      Token(TokenType::kReturn, "return"),
      Token(TokenType::kTrue, "true"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kRightBrace, "}"),
      Token(TokenType::kElse, "else"),
      Token(TokenType::kLeftBrace, "{"),
      Token(TokenType::kReturn, "return"),
      Token(TokenType::kFalse, "false"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kRightBrace, "}"),
      Token(TokenType::kInteger, "10"),
      Token(TokenType::kEqual, "=="),
      Token(TokenType::kInteger, "10"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kInteger, "10"),
      Token(TokenType::kNotEqual, "!="),
      Token(TokenType::kInteger, "9"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kString, "foobar"),
      Token(TokenType::kString, "foo bar"),
      Token(TokenType::kLeftBracket, "["),
      Token(TokenType::kInteger, "1"),
      Token(TokenType::kComma, ","),
      Token(TokenType::kInteger, "2"),
      Token(TokenType::kRightBracket, "]"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kLeftBrace, "{"),
      Token(TokenType::kString, "foo"),
      Token(TokenType::kColon, ":"),
      Token(TokenType::kString, "bar"),
      Token(TokenType::kRightBrace, "}"),
      Token(TokenType::kMacro, "macro"),
      Token(TokenType::kLeftParen, "("),
      Token(TokenType::kIdentifer, "x"),
      Token(TokenType::kComma, ","),
      Token(TokenType::kIdentifer, "y"),
      Token(TokenType::kRightParen, ")"),
      Token(TokenType::kLeftBrace, "{"),
      Token(TokenType::kIdentifer, "x"),
      Token(TokenType::kPlus, "+"),
      Token(TokenType::kIdentifer, "y"),
      Token(TokenType::kSemicolon, ";"),
      Token(TokenType::kRightBrace, "}"),
      Token(TokenType::kSemicolon, ";"),
  };
  ASSERT_TRUE(std::ranges::equal(lexer, expected));
}

}  // namespace monkey::lexer

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}