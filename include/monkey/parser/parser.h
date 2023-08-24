#ifndef MONKEY_PARSER_PARSER_H_
#define MONKEY_PARSER_PARSER_H_

#include <monkey/parser/expr.h>
#include <monkey/parser/stmt.h>

#include <vector>

namespace monkey::parser {

class Parser {
 public:
  explicit Parser(const lexer::Lexer& lexer);
  explicit Parser(const lexer::Lexer&& lexer);

  std::unique_ptr<ast::Program> parse_program();

 private:
  std::unique_ptr<Reader> reader_;
};

}  // namespace monkey::parser

#endif  // MONKEY_PARSER_PARSER_H_