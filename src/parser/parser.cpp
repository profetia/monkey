#include <monkey/parser/parser.h>

#include <utility>

namespace monkey::parser {

Parser::Parser(const lexer::Lexer &lexer)
    : reader_(std::make_unique<Reader>(lexer.begin(), lexer.end())) {}

Parser::Parser(const lexer::Lexer &&lexer) : Parser(lexer) {}

std::unique_ptr<ast::Program> Parser::parse_program() {
  std::vector<std::unique_ptr<ast::Statement>> statements;
  while (!reader_->current_token_is(lexer::TokenType::kEOF)) {
    auto statement = parse_statement(*reader_);
    if (statement) {
      statements.push_back(std::move(statement));
    }
    reader_->next_token();
  }
  return std::make_unique<ast::Program>(std::move(statements));
}

}  // namespace monkey::parser