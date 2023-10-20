#ifndef MONKEY_PARSER_READER_H_
#define MONKEY_PARSER_READER_H_

#include <monkey/ast/ast.h>
#include <monkey/lexer/lexer.h>

namespace monkey::parser {

class Reader {
 public:
  Reader() = default;
  Reader(lexer::Lexer::Iterator current_location,
         lexer::Lexer::Iterator end_location);

  bool operator==(const Reader& rhs) const;
  bool operator!=(const Reader& rhs) const;

  void next_token();

  [[nodiscard]] lexer::Token current_token() const;
  [[nodiscard]] lexer::Token peek_token() const;

  [[nodiscard]] bool expect_peek(lexer::TokenType type);
  [[nodiscard]] bool current_token_is(lexer::TokenType type) const;
  [[nodiscard]] bool peek_token_is(lexer::TokenType type) const;

 private:
  lexer::Lexer::Iterator current_location_;
  lexer::Lexer::Iterator peek_location_;
  lexer::Lexer::Iterator end_location_;
};

}  // namespace monkey::parser

#endif  // MONKEY_PARSER_READER_H_