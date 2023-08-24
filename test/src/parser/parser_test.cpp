#include <gtest/gtest.h>
#include <monkey/parser/error.h>
#include <monkey/parser/parser.h>

#include <concepts>
#include <ranges>

namespace monkey::parser {

template <typename T, typename... Args>
  requires std::derived_from<T, ast::Node> &&
           std::conjunction_v<std::is_base_of<T, Args>...>
std::vector<std::unique_ptr<T>> make_vector(Args&&... args) {
  auto vec = std::vector<std::unique_ptr<T>>{};
  (vec.push_back(std::make_unique<Args>(std::move(args))), ...);
  return vec;
}

template <typename... Args>
  requires std::conjunction_v<
      std::is_convertible<Args, std::pair<std::unique_ptr<ast::Expression>,
                                          std::unique_ptr<ast::Expression>>>...>
std::unordered_map<std::unique_ptr<ast::Expression>,
                   std::unique_ptr<ast::Expression>>
make_map(Args&&... args) {
  auto map = std::unordered_map<std::unique_ptr<ast::Expression>,
                                std::unique_ptr<ast::Expression>>{};
  (map.emplace(std::move(args.first), std::move(args.second)), ...);
  return map;
}

TEST(MonkeyParserTest, LetStatements) {
  auto inputs = std::vector<std::string>{"let x = 5;", "let y = true;",
                                         "let foobar = y;"};
  auto expects = make_vector<ast::Program>(
      ast::Program(make_vector<ast::Statement>(
          ast::LetStatement(std::make_unique<ast::Identifier>("x"),
                            std::make_unique<ast::IntegerLiteral>(5)))),
      ast::Program(make_vector<ast::Statement>(
          ast::LetStatement(std::make_unique<ast::Identifier>("y"),
                            std::make_unique<ast::BooleanLiteral>(true)))),
      ast::Program(make_vector<ast::Statement>(
          ast::LetStatement(std::make_unique<ast::Identifier>("foobar"),
                            std::make_unique<ast::Identifier>("y")))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

TEST(MonkeyParserTest, ReturnStatement) {
  auto inputs =
      std::vector<std::string>{"return 5;", "return 10;", "return 993322;"};
  auto expects = make_vector<ast::Program>(
      ast::Program(make_vector<ast::Statement>(
          ast::ReturnStatement(std::make_unique<ast::IntegerLiteral>(5)))),
      ast::Program(make_vector<ast::Statement>(
          ast::ReturnStatement(std::make_unique<ast::IntegerLiteral>(10)))),
      ast::Program(make_vector<ast::Statement>(ast::ReturnStatement(
          std::make_unique<ast::IntegerLiteral>(993322)))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

TEST(MonkeyParserTest, IdentifierExpression) {
  auto inputs = std::vector<std::string>{"foobar;", "foo;bar"};
  auto expects = make_vector<ast::Program>(
      ast::Program(make_vector<ast::Statement>(ast::ExpressionStatement(
          std::make_unique<ast::Identifier>("foobar")))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::Identifier>("foo")),
          ast::ExpressionStatement(std::make_unique<ast::Identifier>("bar")))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

TEST(MonkeyParserTest, IntegerLiteralExpression) {
  auto inputs = std::vector<std::string>{"5", "10;", "993322;1"};
  auto expects = make_vector<ast::Program>(
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::IntegerLiteral>(5)))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::IntegerLiteral>(10)))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(
              std::make_unique<ast::IntegerLiteral>(993322)),
          ast::ExpressionStatement(std::make_unique<ast::IntegerLiteral>(1)))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

TEST(MonkeyParserTest, BooleanLiteralExpression) {
  auto inputs = std::vector<std::string>{"true", "false;", "true;false"};
  auto expects = make_vector<ast::Program>(
      ast::Program(make_vector<ast::Statement>(ast::ExpressionStatement(
          std::make_unique<ast::BooleanLiteral>(true)))),
      ast::Program(make_vector<ast::Statement>(ast::ExpressionStatement(
          std::make_unique<ast::BooleanLiteral>(false)))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::BooleanLiteral>(true)),
          ast::ExpressionStatement(
              std::make_unique<ast::BooleanLiteral>(false)))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

TEST(MonkeyParserTest, PrefixExpression) {
  auto inputs =
      std::vector<std::string>{"-5", "!10;", "-993322;1", "!true", "!false;"};
  auto expects = make_vector<ast::Program>(
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::PrefixExpression>(
              lexer::TokenType::kMinus,
              std::make_unique<ast::IntegerLiteral>(5))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::PrefixExpression>(
              lexer::TokenType::kBang,
              std::make_unique<ast::IntegerLiteral>(10))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::PrefixExpression>(
              lexer::TokenType::kMinus,
              std::make_unique<ast::IntegerLiteral>(993322))),
          ast::ExpressionStatement(std::make_unique<ast::IntegerLiteral>(1)))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::PrefixExpression>(
              lexer::TokenType::kBang,
              std::make_unique<ast::BooleanLiteral>(true))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::PrefixExpression>(
              lexer::TokenType::kBang,
              std::make_unique<ast::BooleanLiteral>(false))))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

TEST(MonkeyParserTest, InfixExpression) {
  auto inputs = std::vector<std::string>{"5 + 5", "5 - 5", "5 * 5",  "5 / 5",
                                         "5 > 5", "5 < 5", "5 == 5", "5 != 5"};
  auto expects = make_vector<ast::Program>(
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::InfixExpression>(
              std::make_unique<ast::IntegerLiteral>(5), lexer::TokenType::kPlus,
              std::make_unique<ast::IntegerLiteral>(5))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::InfixExpression>(
              std::make_unique<ast::IntegerLiteral>(5),
              lexer::TokenType::kMinus,
              std::make_unique<ast::IntegerLiteral>(5))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::InfixExpression>(
              std::make_unique<ast::IntegerLiteral>(5),
              lexer::TokenType::kAsterisk,
              std::make_unique<ast::IntegerLiteral>(5))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::InfixExpression>(
              std::make_unique<ast::IntegerLiteral>(5),
              lexer::TokenType::kSlash,
              std::make_unique<ast::IntegerLiteral>(5))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::InfixExpression>(
              std::make_unique<ast::IntegerLiteral>(5),
              lexer::TokenType::kGreaterThan,
              std::make_unique<ast::IntegerLiteral>(5))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::InfixExpression>(
              std::make_unique<ast::IntegerLiteral>(5),
              lexer::TokenType::kLessThan,
              std::make_unique<ast::IntegerLiteral>(5))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::InfixExpression>(
              std::make_unique<ast::IntegerLiteral>(5),
              lexer::TokenType::kEqual,
              std::make_unique<ast::IntegerLiteral>(5))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::InfixExpression>(
              std::make_unique<ast::IntegerLiteral>(5),
              lexer::TokenType::kNotEqual,
              std::make_unique<ast::IntegerLiteral>(5))))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

TEST(MonkeyParserTest, OperatorPrecedence) {
  auto inputs =
      std::vector<std::string>{"-a * b",
                               "!-a",
                               "a + b - c",
                               "a * b * c",
                               "a * b / c",
                               "a + b / c",
                               "a + b * c + d / e - f",
                               "3 + 4; -5 * 5",
                               "5 > 4 == 3 < 4",
                               "5 < 4 != 3 > 4",
                               "3 + 4 * 5 == 3 * 1 + 4 * 5",
                               "1 + (2 + 3) + 4",
                               "(5 + 5) * 2",
                               "2 / (5 + 5)",
                               "-(5 + 5)",
                               "!(true == true)",
                               "a + add(b * c) + d",
                               "add(a, b, 1, 2 * 3, 4 + 5, add(6, 7 * 8))",
                               "add(a + b + c * d / f + g)",
                               "[1, 2, 3][1]",
                               "a * [1, 2, 3, 4][b * c] * d",
                               "add(a * b[2], b[1], 2 * [1, 2][1])"};

  auto expects = std::vector<std::string>{
      "((-a) * b)",
      "(!(-a))",
      "((a + b) - c)",
      "((a * b) * c)",
      "((a * b) / c)",
      "(a + (b / c))",
      "(((a + (b * c)) + (d / e)) - f)",
      "(3 + 4)\n((-5) * 5)",
      "((5 > 4) == (3 < 4))",
      "((5 < 4) != (3 > 4))",
      "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))",
      "((1 + (2 + 3)) + 4)",
      "((5 + 5) * 2)",
      "(2 / (5 + 5))",
      "(-(5 + 5))",
      "(!(true == true))",
      "((a + add((b * c))) + d)",
      "add(a, b, 1, (2 * 3), (4 + 5), add(6, (7 * 8)))",
      "add((((a + b) + ((c * d) / f)) + g))",
      "([1, 2, 3][1])",
      "((a * ([1, 2, 3, 4][(b * c)])) * d)",
      "add((a * (b[2])), (b[1]), (2 * ([1, 2][1])))",
  };

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->to_string() == rhs;
      }));
}

TEST(MonkeyParserTest, FunctionLiteralExpression) {
  auto inputs = std::vector<std::string>{"fn(x, y) { x + y; }", "fn() {};",
                                         "fn(x) {};", "fn(x, y, z) {};"};
  auto expects = make_vector<ast::Program>(
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::FunctionLiteral>(
              make_vector<ast::Identifier>(ast::Identifier("x"),
                                           ast::Identifier("y")),
              std::make_unique<ast::BlockStatement>(
                  make_vector<ast::Statement>(ast::ExpressionStatement(
                      std::make_unique<ast::InfixExpression>(
                          std::make_unique<ast::Identifier>("x"),
                          lexer::TokenType::kPlus,
                          std::make_unique<ast::Identifier>("y"))))))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::FunctionLiteral>(
              make_vector<ast::Identifier>(),
              std::make_unique<ast::BlockStatement>(
                  make_vector<ast::Statement>()))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::FunctionLiteral>(
              make_vector<ast::Identifier>(ast::Identifier("x")),
              std::make_unique<ast::BlockStatement>(
                  make_vector<ast::Statement>()))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::FunctionLiteral>(
              make_vector<ast::Identifier>(ast::Identifier("x"),
                                           ast::Identifier("y"),
                                           ast::Identifier("z")),
              std::make_unique<ast::BlockStatement>(
                  make_vector<ast::Statement>()))))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

TEST(MonkeyParserTest, HashLiteralExpression) {
  auto inputs = std::vector<std::string>{
      "{}", "{5: 6, 3: 4, 1: 2}",
      R"({"one": 0 + 1, "two": 10 - 8, "three": 15 / 5})"};
  auto expects = make_vector<ast::Program>(
      ast::Program(make_vector<ast::Statement>(ast::ExpressionStatement(
          std::make_unique<ast::HashLiteral>(make_map())))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::HashLiteral>(make_map(
              std::make_pair(std::make_unique<ast::IntegerLiteral>(5),
                             std::make_unique<ast::IntegerLiteral>(6)),
              std::make_pair(std::make_unique<ast::IntegerLiteral>(1),
                             std::make_unique<ast::IntegerLiteral>(2)),
              std::make_pair(std::make_unique<ast::IntegerLiteral>(3),
                             std::make_unique<ast::IntegerLiteral>(4))))))),
      ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::HashLiteral>(make_map(
              std::make_pair(std::make_unique<ast::StringLiteral>("one"),
                             std::make_unique<ast::InfixExpression>(
                                 std::make_unique<ast::IntegerLiteral>(0),
                                 lexer::TokenType::kPlus,
                                 std::make_unique<ast::IntegerLiteral>(1))),
              std::make_pair(std::make_unique<ast::StringLiteral>("two"),
                             std::make_unique<ast::InfixExpression>(
                                 std::make_unique<ast::IntegerLiteral>(10),
                                 lexer::TokenType::kMinus,
                                 std::make_unique<ast::IntegerLiteral>(8))),
              std::make_pair(
                  std::make_unique<ast::StringLiteral>("three"),
                  std::make_unique<ast::InfixExpression>(
                      std::make_unique<ast::IntegerLiteral>(15),
                      lexer::TokenType::kSlash,
                      std::make_unique<ast::IntegerLiteral>(5)))))))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

TEST(MonkeyParserTest, MacroLiteralExpression) {
  auto inputs = std::vector<std::string>{"macro(x, y) { x + y; }"};
  auto expects =
      make_vector<ast::Program>(ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::MacroLiteral>(
              make_vector<ast::Identifier>(ast::Identifier("x"),
                                           ast::Identifier("y")),
              std::make_unique<ast::BlockStatement>(
                  make_vector<ast::Statement>(ast::ExpressionStatement(
                      std::make_unique<ast::InfixExpression>(
                          std::make_unique<ast::Identifier>("x"),
                          lexer::TokenType::kPlus,
                          std::make_unique<ast::Identifier>("y"))))))))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

TEST(MonkeyParserTest, IfExpression) {
  auto inputs = std::vector<std::string>{"if (x < y) { x } else { y }"};
  auto expects =
      make_vector<ast::Program>(ast::Program(make_vector<ast::Statement>(
          ast::ExpressionStatement(std::make_unique<ast::IfExpression>(
              std::make_unique<ast::InfixExpression>(
                  std::make_unique<ast::Identifier>("x"),
                  lexer::TokenType::kLessThan,
                  std::make_unique<ast::Identifier>("y")),
              std::make_unique<ast::BlockStatement>(
                  make_vector<ast::Statement>(ast::ExpressionStatement(
                      std::make_unique<ast::Identifier>("x")))),
              std::make_unique<ast::BlockStatement>(
                  make_vector<ast::Statement>(ast::ExpressionStatement(
                      std::make_unique<ast::Identifier>("y")))))))));

  ASSERT_TRUE(
      std::ranges::equal(inputs, expects, [](const auto& lhs, const auto& rhs) {
        auto lexer = lexer::Lexer(lhs);
        auto program = Parser(lexer).parse_program();
        return program->operator==(*rhs);
      }));
}

}  // namespace monkey::parser

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}