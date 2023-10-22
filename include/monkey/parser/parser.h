#ifndef MONKEY_PARSER_PARSER_H_
#define MONKEY_PARSER_PARSER_H_

#include <monkey/parser/expr.h>
#include <monkey/parser/stmt.h>

#include <vector>

namespace monkey::parser {

class Parser {
 public:
  explicit Parser(const lexer::Lexer& lexer);

  std::shared_ptr<ast::Program> parse_program();

 private:
  std::shared_ptr<Reader> reader_;
};

}  // namespace monkey::parser

#endif  // MONKEY_PARSER_PARSER_H_