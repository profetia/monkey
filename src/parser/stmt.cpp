#include <monkey/ast/expr.h>
#include <monkey/parser/expr.h>
#include <monkey/parser/stmt.h>

namespace monkey::parser {

std::unique_ptr<ast::Statement> parse_statement(Reader& reader) {
  switch (reader.current_token().type()) {
    case lexer::TokenType::kLet:
      return parse_let_statement(reader);
    case lexer::TokenType::kReturn:
      return parse_return_statement(reader);
    default:
      return parse_expression_statement(reader);
  }
}

std::unique_ptr<ast::LetStatement> parse_let_statement(Reader& reader) {
  if (!reader.expect_peek(lexer::TokenType::kIdentifer)) {
    return nullptr;
  }
  auto name =
      std::make_unique<ast::Identifier>(reader.current_token().literal());
  if (!reader.expect_peek(lexer::TokenType::kAssign)) {
    return nullptr;
  }
  reader.next_token();
  auto value = parse_expression(reader, Precedence::kLowest);
  if (reader.peek_token_is(lexer::TokenType::kSemicolon)) {
    reader.next_token();
  }
  return std::make_unique<ast::LetStatement>(std::move(name), std::move(value));
}

std::unique_ptr<ast::ReturnStatement> parse_return_statement(Reader& reader) {
  reader.next_token();
  auto return_value = parse_expression(reader, Precedence::kLowest);
  if (reader.peek_token_is(lexer::TokenType::kSemicolon)) {
    reader.next_token();
  }
  return std::make_unique<ast::ReturnStatement>(std::move(return_value));
}

std::unique_ptr<ast::ExpressionStatement> parse_expression_statement(
    Reader& reader) {
  auto expression = parse_expression(reader, Precedence::kLowest);
  if (reader.peek_token_is(lexer::TokenType::kSemicolon)) {
    reader.next_token();
  }
  return std::make_unique<ast::ExpressionStatement>(std::move(expression));
}

std::unique_ptr<ast::BlockStatement> parse_block_statement(Reader& reader) {
  auto statements = std::vector<std::unique_ptr<ast::Statement>>();
  reader.next_token();
  while (!reader.current_token_is(lexer::TokenType::kRightBrace) &&
         !reader.current_token_is(lexer::TokenType::kEOF)) {
    auto statement = parse_statement(reader);
    if (statement) {
      statements.push_back(std::move(statement));
    }
    reader.next_token();
  }
  return std::make_unique<ast::BlockStatement>(std::move(statements));
}

}  // namespace monkey::parser