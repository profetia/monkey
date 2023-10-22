#include <gtest/gtest.h>
#include <monkey/eval/eval.h>
#include <monkey/lexer/lexer.h>
#include <monkey/object/env.h>
#include <monkey/parser/parser.h>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace monkey::eval {

TEST(MonkeyEvalTest, IntegerLiteral) {
  auto inputs = std::vector<std::string>{"5",
                                         "10",
                                         "-5",
                                         "-10",
                                         "5 + 5 + 5 + 5 - 10",
                                         "2 * 2 * 2 * 2 * 2",
                                         "-50 + 100 + -50",
                                         "5 * 2 + 10",
                                         "5 + 2 * 10",
                                         "20 + 2 * -10",
                                         "50 / 2 * 2 + 10",
                                         "2 * (5 + 10)",
                                         "3 * 3 * 3 + 10",
                                         "3 * (3 * 3) + 10",
                                         "(5 + 10 * 2 + 15 / 3) * 2 + -10"};
  auto expecteds = std::vector<int64_t>{5,  10, -5, -10, 10, 32, 0, 20,
                                        25, 0,  60, 30,  37, 37, 50};
  ASSERT_TRUE(std::ranges::equal(
      inputs, expecteds, [](const auto& input, const auto& expected) {
        auto l = lexer::Lexer(input);
        auto p = parser::Parser(l);
        auto program = p.parse_program();
        auto env = std::make_shared<object::Env>();
        return eval(*program, env)->to_string() == std::to_string(expected);
      }));
}

TEST(MonkeyEvalTest, BooleanLiteral) {
  auto inputs = std::vector<std::string>{
      "true",  "false",  "1 < 2",  "1 > 2",  "1 < 1",
      "1 > 1", "1 == 1", "1 != 1", "1 == 2", "1 != 2",
  };
  auto expecteds = std::vector<bool>{true,  false, true,  false, false,
                                     false, true,  false, false, true};
  ASSERT_TRUE(std::ranges::equal(inputs, expecteds,
                                 [](const auto& input, const auto& expected) {
                                   auto l = lexer::Lexer(input);
                                   auto p = parser::Parser(l);
                                   auto program = p.parse_program();
                                   auto env = std::make_shared<object::Env>();
                                   return eval(*program, env)->to_string() ==
                                          (expected ? "true" : "false");
                                 }));
}

TEST(MonkeyEvalTest, PrefixExpression) {
  auto inputs = std::vector<std::string>{
      "!true", "!false", "!!true", "!!false",   "!5",          "!!5",
      "-5",    "-10",    "--10",   "-(-(-10))", "-(-(-(-10)))"};
  auto expecteds = std::vector<std::string>{"false", "true", "true", "false",
                                            "false", "true", "-5",   "-10",
                                            "10",    "-10",  "10"};
  ASSERT_TRUE(std::ranges::equal(
      inputs, expecteds, [](const auto& input, const auto& expected) {
        auto l = lexer::Lexer(input);
        auto p = parser::Parser(l);
        auto program = p.parse_program();
        auto env = std::make_shared<object::Env>();
        return eval(*program, env)->to_string() == expected;
      }));
}

TEST(MonkeyEvalTest, IfExpression) {
  auto inputs = std::vector<std::string>{"if (true) { 10 }",
                                         "if (false) { 10 }",
                                         "if (1) { 10 }",
                                         "if (1 < 2) { 10 }",
                                         "if (1 > 2) { 10 }",
                                         "if (1 > 2) { 10 } else { 20 }",
                                         "if (1 < 2) { 10 } else { 20 }"};
  auto expecteds =
      std::vector<std::string>{"10", "null", "10", "10", "null", "20", "10"};
  ASSERT_TRUE(std::ranges::equal(
      inputs, expecteds, [](const auto& input, const auto& expected) {
        auto l = lexer::Lexer(input);
        auto p = parser::Parser(l);
        auto program = p.parse_program();
        auto env = std::make_shared<object::Env>();
        return eval(*program, env)->to_string() == expected;
      }));
}

TEST(MonkeyEvalTest, ReturnStatement) {
  auto inputs = std::vector<std::string>{
      "return 10;", "return 10; 9;", "return 2 * 5; 9;", "9; return 2 * 5; 9;",
      R"(
        if (10 > 1) {
          if (10 > 1) {
            return 10;
          }
          return 1;
        }
      )",
  };
  auto expecteds = std::vector<int64_t>{10, 10, 10, 10, 10};
  ASSERT_TRUE(std::ranges::equal(
      inputs, expecteds, [](const auto& input, const auto& expected) {
        auto l = lexer::Lexer(input);
        auto p = parser::Parser(l);
        auto program = p.parse_program();
        auto env = std::make_shared<object::Env>();
        return eval(*program, env)->to_string() == std::to_string(expected);
      }));
}

TEST(MonkeyEvalTest, ErrorHandling) {
  auto inputs = std::vector<std::string>{"5 + true;",
                                         "5 + true; 5;",
                                         "-true",
                                         "true + false;",
                                         "5; true + false; 5",
                                         "if (10 > 1) { true + false; }",
                                         R"(
        if (10 > 1) {
          if (10 > 1) {
            return true + false;
          }
          return 1;
        }
      )",
                                         "foobar",
                                         R"(
        let f = if (1 < 2) { let y = 1; } else { 2 }; y;
      )",
                                         R"(
        "Hello" - "World"
      )",
                                         R"(
        {"name": "Monkey"}[fn(x) { x }]
      )"};
  auto expecteds = std::vector<std::string>{
      "ERROR: wrong operand types for +: INTEGER + BOOLEAN",
      "ERROR: wrong operand types for +: INTEGER + BOOLEAN",
      "ERROR: wrong operand type for -: -BOOLEAN",
      "ERROR: wrong operand types for +: BOOLEAN + BOOLEAN",
      "ERROR: wrong operand types for +: BOOLEAN + BOOLEAN",
      "ERROR: wrong operand types for +: BOOLEAN + BOOLEAN",
      "ERROR: wrong operand types for +: BOOLEAN + BOOLEAN",
      "ERROR: identifier not found: foobar",
      "ERROR: identifier not found: y",
      "ERROR: wrong operand types for -: STRING - STRING",
      "ERROR: wrong index types for []: HASH[FUNCTION]",
  };
  ASSERT_TRUE(std::ranges::equal(
      inputs, expecteds, [](const auto& input, const auto& expected) {
        auto l = lexer::Lexer(input);
        auto p = parser::Parser(l);
        auto program = p.parse_program();
        auto env = std::make_shared<object::Env>();
        return eval(*program, env)->to_string() == expected;
      }));
}

TEST(MonkeyEvalTest, LetStatement) {
  auto inputs = std::vector<std::string>{
      "let a = 5; a;", "let a = 5 * 5; a;", "let a = 5; let b = a; b;",
      "let a = 5; let b = a; let c = a + b + 5; c;"};
  auto expecteds = std::vector<int64_t>{5, 25, 5, 15};
  ASSERT_TRUE(std::ranges::equal(
      inputs, expecteds, [](const auto& input, const auto& expected) {
        auto l = lexer::Lexer(input);
        auto p = parser::Parser(l);
        auto program = p.parse_program();
        auto env = std::make_shared<object::Env>();
        return eval(*program, env)->to_string() == std::to_string(expected);
      }));
}

TEST(MonkeyEvalTest, CallExpression) {
  auto inputs = std::vector<std::string>{
      "let identity = fn(x) { x; }; identity(5);",
      "let identity = fn(x) { return x; }; identity(5);",
      "let double = fn(x) { x * 2; }; double(5);",
      "let add = fn(x, y) { x + y; }; add(5, 5);",
      "let add = fn(x, y) { x + y; }; add(5 + 5, add(5, 5));",
      "fn(x) { x; }(5)",
      R"(
        let newAdder = fn(x) {
          fn(y) { x + y };
        };
        let addTwo = newAdder(2);
        addTwo(2);
      )",
  };
  auto expecteds = std::vector<int64_t>{5, 5, 10, 10, 20, 5, 4};
  ASSERT_TRUE(std::ranges::equal(
      inputs, expecteds, [](const auto& input, const auto& expected) {
        auto l = lexer::Lexer(input);
        auto p = parser::Parser(l);
        auto program = p.parse_program();
        auto env = std::make_shared<object::Env>();
        return eval(*program, env)->to_string() == std::to_string(expected);
      }));
}

TEST(MonkeyEvalTest, StringLiteral) {
  auto inputs = std::vector<std::string>{
      R"("Hello World!")",
      R"("Hello" + " " + "World!")",
  };
  auto expecteds = std::vector<std::string>{"Hello World!", "Hello World!"};
  ASSERT_TRUE(std::ranges::equal(
      inputs, expecteds, [](const auto& input, const auto& expected) {
        auto l = lexer::Lexer(input);
        auto p = parser::Parser(l);
        auto program = p.parse_program();
        auto env = std::make_shared<object::Env>();
        return eval(*program, env)->to_string() == expected;
      }));
}

TEST(MonkeyEvalTest, ArrayLiteral) {
  auto inputs =
      std::vector<std::string>{"[]", "[1, 2, 3]", "[1 + 2, 3 * 4, 5 + 6]"};
  auto expecteds = std::vector<std::string>{
      "[]",
      "[1, 2, 3, ]",
      "[3, 12, 11, ]",
  };
  ASSERT_TRUE(std::ranges::equal(inputs, expecteds,
                                 [](const auto& input, const auto& expected) {
                                   auto l = lexer::Lexer(input);
                                   auto p = parser::Parser(l);
                                   auto program = p.parse_program();
                                   auto env = std::make_shared<object::Env>();
                                   auto result = eval(*program, env);
                                   return result->to_string() == expected;
                                 }));
}

TEST(MonkeyEvalTest, IndexExpression) {
  auto inputs = std::vector<std::string>{
      "[1, 2, 3][0]",
      "[1, 2, 3][1]",
      "[1, 2, 3][2]",
      "let i = 0; [1][i];",
      "[1, 2, 3][1 + 1];",
      "let myArray = [1, 2, 3]; myArray[2];",
      "let myArray = [1, 2, 3]; myArray[0] + myArray[1] + myArray[2];",
      "let myArray = [1, 2, 3]; let i = myArray[0]; myArray[i];",
      "[1, 2, 3][3]",
      "[1, 2, 3][-1]",
      R"(
        {"foo": 5}["foo"]
      )",
      R"(
        {"foo": 5}["bar"]
      )",
      R"(
        let key = "foo"; {"foo": 5}[key]
      )",
      R"(
        {}["foo"]
      )",
      "{5: 5}[5]",
      "{true: 5}[true]",
      "{false: 5}[false]",
  };
  auto expecteds = std::vector<std::string>{
      "1",    "2", "3",    "1", "3",    "3", "6", "2", "null",
      "null", "5", "null", "5", "null", "5", "5", "5",
  };
  ASSERT_TRUE(std::ranges::equal(inputs, expecteds,
                                 [](const auto& input, const auto& expected) {
                                   auto l = lexer::Lexer(input);
                                   auto p = parser::Parser(l);
                                   auto program = p.parse_program();
                                   auto env = std::make_shared<object::Env>();
                                   auto result = eval(*program, env);
                                   return result->to_string() == expected;
                                 }));
}

TEST(MonkeyEvalTest, HashLiteral) {
  auto inputs = std::vector<std::string>{
      "{}",
      "{1: 2, 2: 3}",
      "{1 + 1: 2 * 2, 3 + 3: 4 * 4}",
  };
  auto expecteds = std::vector<std::string>{
      "{}",
      "{1: 2, 2: 3, }",
      "{2: 4, 6: 16, }",
  };
  ASSERT_TRUE(std::ranges::equal(inputs, expecteds,
                                 [](const auto& input, const auto& expected) {
                                   auto l = lexer::Lexer(input);
                                   auto p = parser::Parser(l);
                                   auto program = p.parse_program();
                                   auto env = std::make_shared<object::Env>();
                                   auto result = eval(*program, env);
                                   return result->to_string() == expected;
                                 }));
}

TEST(MonkeyEvalTest, Builtins) {
  auto inputs = std::vector<std::string>{
      "len(\"\")",      "len(\"four\")", "len(\"hello world\")",
      "len([1, 2, 3])", "len(1)",        R"(len("one", "two"))",
  };
  auto expecteds = std::vector<std::string>{
      "0",
      "4",
      "11",
      "3",
      "ERROR: wrong argument type for len: expected STRING, got INTEGER",
      "ERROR: wrong number of arguments for len: expected 1, got 2",
  };
  ASSERT_TRUE(std::ranges::equal(inputs, expecteds,
                                 [](const auto& input, const auto& expected) {
                                   auto l = lexer::Lexer(input);
                                   auto p = parser::Parser(l);
                                   auto program = p.parse_program();
                                   auto env = std::make_shared<object::Env>();
                                   auto result = eval(*program, env);
                                   return result->to_string() == expected;
                                 }));
}

}  // namespace monkey::eval

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}