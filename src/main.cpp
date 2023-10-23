#include <fmt/core.h>
#include <monkey/ast/ast.h>
#include <monkey/eval/eval.h>
#include <monkey/lexer/lexer.h>
#include <monkey/object/env.h>
#include <monkey/object/object.h>
#include <monkey/parser/parser.h>

#include <exception>
#include <iostream>
#include <memory>
#include <string>

static const std::string kPrompt = ">> ";
static const std::string kMonkey = R"(
            __,__
   .--.  .-"     "-.  .--.
  / .. \/  .-. .-.  \/ .. \
 | |  '|  /   Y   \  |'  | |
 | \   \  \ 0 | 0 /  /   / |
  \ '- ,\.-"""""""-./, -' /
   ''-' /_   ^ ^   _\ '-''
       |  \._   _./  |
       \   \ '~' /   /
        '._ '-=-' _.'
           '-----'
)";

void print_error(const std::string& why) {
  fmt::print("Woops! We ran into some monkey business here!\n");
  fmt::print("{}\n", why);
}

void print_preface() {
  fmt::print("{}\n", kMonkey);
  fmt::print("Welcome to the Monkey programming language!\n");
  fmt::print("See https://monkeylang.org/ for more information\n");
  fmt::print("Feel free to type in commands\n");
}

int main() {
  auto env = std::make_shared<monkey::object::Env>();
  print_preface();
  while (true) {
    fmt::print("{}", kPrompt);
    std::string input;
    std::getline(std::cin, input);
    if (input == "exit") {
      break;
    }

    auto l = monkey::lexer::Lexer(input);
    auto p = monkey::parser::Parser(l);
    std::shared_ptr<monkey::ast::Program> program;
    try {
      program = p.parse_program();
    } catch (const std::exception& e) {
      print_error(e.what());
      continue;
    }

    auto evaluated = monkey::eval::eval(*program, env);
    if (evaluated != nullptr) {
      if (evaluated->type() == monkey::object::ObjectType::kError) {
        print_error("RuntimeError: " +
                    dynamic_cast<monkey::object::Error&>(*evaluated).message());
        continue;
      }
      fmt::print("{}\n", evaluated->to_string());
    }
  }
  return 0;
}
