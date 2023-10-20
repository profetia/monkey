#include <monkey/ast/ast.h>
#include <monkey/ast/stmt.h>
#include <monkey/lexer/lexer.h>
#include <monkey/lexer/token.h>
#include <monkey/parser/parser.h>
#include <monkey/parser/reader.h>
#include <monkey/parser/stmt.h>

#include <memory>
#include <utility>
#include <vector>

namespace monkey::parser {

Parser::Parser(const lexer::Lexer &lexer)
    : reader_(std::make_unique<Reader>(lexer.begin(), lexer.end())) {}

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