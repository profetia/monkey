#include <monkey/ast/expr.h>
#include <monkey/ast/stmt.h>
#include <monkey/lexer/token.h>
#include <monkey/parser/error.h>
#include <monkey/parser/expr.h>
#include <monkey/parser/reader.h>
#include <monkey/parser/stmt.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace monkey::parser {

Precedence get_precedence(lexer::TokenType type) {
  if (kPrecedences.find(type) != kPrecedences.end()) {
    return kPrecedences.at(type);
  }

  return Precedence::kLowest;
}

std::shared_ptr<ast::Expression> parse_expression(Reader& reader,
                                                  Precedence precedence) {
  auto prefix_handler = kPrefixHandlers.find(reader.current_token().type());
  if (prefix_handler == kPrefixHandlers.end()) {
    throw HandlerNotFoundError(reader.current_token().type());
  }

  auto left_expression = prefix_handler->second(reader);
  while (!reader.peek_token_is(lexer::TokenType::kSemicolon) &&
         precedence < get_precedence(reader.peek_token().type())) {
    auto infix_handler = kInfixHandlers.find(reader.peek_token().type());
    if (infix_handler == kInfixHandlers.end()) {
      return left_expression;
    }

    reader.next_token();
    left_expression = infix_handler->second(reader, std::move(left_expression));
  }
  return left_expression;
}

std::shared_ptr<ast::Identifier> parse_identifier(Reader& reader) {
  return std::make_unique<ast::Identifier>(reader.current_token().literal());
}

std::shared_ptr<ast::IntegerLiteral> parse_integer_literal(Reader& reader) {
  try {
    auto value = std::stoi(reader.current_token().literal());
    return std::make_unique<ast::IntegerLiteral>(value);
  } catch (std::invalid_argument& e) {
    throw InvalidIntegerError(reader.current_token().literal());
  }
}

std::shared_ptr<ast::BooleanLiteral> parse_boolean_literal(Reader& reader) {
  return std::make_unique<ast::BooleanLiteral>(
      reader.current_token_is(lexer::TokenType::kTrue));
}

std::shared_ptr<ast::StringLiteral> parse_string_literal(Reader& reader) {
  return std::make_unique<ast::StringLiteral>(reader.current_token().literal());
}

std::shared_ptr<ast::ArrayLiteral> parse_array_literal(Reader& reader) {
  std::vector<std::shared_ptr<ast::Expression>> elements =
      parse_expression_list(reader, lexer::TokenType::kRightBracket);
  return std::make_unique<ast::ArrayLiteral>(std::move(elements));
}

std::shared_ptr<ast::HashLiteral> parse_hash_literal(Reader& reader) {
  std::unordered_map<std::shared_ptr<ast::Expression>,
                     std::shared_ptr<ast::Expression>>
      pairs;
  while (!reader.peek_token_is(lexer::TokenType::kRightBrace)) {
    reader.next_token();
    auto key = parse_expression(reader, Precedence::kLowest);
    if (!reader.expect_peek(lexer::TokenType::kColon)) {
      return nullptr;
    }
    reader.next_token();
    auto value = parse_expression(reader, Precedence::kLowest);
    pairs.emplace(std::move(key), std::move(value));
    if (!reader.peek_token_is(lexer::TokenType::kRightBrace) &&
        !reader.expect_peek(lexer::TokenType::kComma)) {
      return nullptr;
    }
  }
  if (!reader.expect_peek(lexer::TokenType::kRightBrace)) {
    return nullptr;
  }
  return std::make_unique<ast::HashLiteral>(std::move(pairs));
}

std::shared_ptr<ast::FunctionLiteral> parse_function_literal(Reader& reader) {
  if (!reader.expect_peek(lexer::TokenType::kLeftParen)) {
    return nullptr;
  }
  auto parameters = parse_function_parameters(reader);
  if (!reader.expect_peek(lexer::TokenType::kLeftBrace)) {
    return nullptr;
  }
  auto body = parse_block_statement(reader);
  return std::make_unique<ast::FunctionLiteral>(std::move(parameters),
                                                std::move(body));
}

std::shared_ptr<ast::PrefixExpression> parse_prefix_expression(Reader& reader) {
  auto token = reader.current_token();
  reader.next_token();
  auto right = parse_expression(reader, Precedence::kPrefix);
  return std::make_unique<ast::PrefixExpression>(token.type(),
                                                 std::move(right));
}

std::shared_ptr<ast::InfixExpression> parse_infix_expression(
    Reader& reader, std::shared_ptr<ast::Expression> left) {
  auto token = reader.current_token();
  auto precedence = get_precedence(token.type());
  reader.next_token();
  auto right = parse_expression(reader, precedence);
  return std::make_unique<ast::InfixExpression>(std::move(left), token.type(),
                                                std::move(right));
}

std::shared_ptr<ast::IndexExpression> parse_index_expression(
    Reader& reader, std::shared_ptr<ast::Expression> left) {
  reader.next_token();
  auto index = parse_expression(reader, Precedence::kLowest);
  if (!reader.expect_peek(lexer::TokenType::kRightBracket)) {
    return nullptr;
  }
  return std::make_unique<ast::IndexExpression>(std::move(left),
                                                std::move(index));
}

std::shared_ptr<ast::IfExpression> parse_if_expression(Reader& reader) {
  auto token = reader.current_token();
  if (!reader.expect_peek(lexer::TokenType::kLeftParen)) {
    return nullptr;
  }
  reader.next_token();
  auto condition = parse_expression(reader, Precedence::kLowest);
  if (!reader.expect_peek(lexer::TokenType::kRightParen)) {
    return nullptr;
  }
  if (!reader.expect_peek(lexer::TokenType::kLeftBrace)) {
    return nullptr;
  }
  auto consequence = parse_block_statement(reader);
  std::shared_ptr<ast::BlockStatement> alternative;
  if (reader.peek_token_is(lexer::TokenType::kElse)) {
    reader.next_token();
    if (!reader.expect_peek(lexer::TokenType::kLeftBrace)) {
      return nullptr;
    }
    alternative = parse_block_statement(reader);
  }
  return std::make_unique<ast::IfExpression>(
      std::move(condition), std::move(consequence), std::move(alternative));
}

std::shared_ptr<ast::CallExpression> parse_call_expression(
    Reader& reader, std::shared_ptr<ast::Expression> function) {
  auto arguments = parse_expression_list(reader, lexer::TokenType::kRightParen);
  return std::make_unique<ast::CallExpression>(std::move(function),
                                               std::move(arguments));
}

std::shared_ptr<ast::Expression> parse_grouped_expression(Reader& reader) {
  reader.next_token();
  auto expression = parse_expression(reader, Precedence::kLowest);
  if (!reader.expect_peek(lexer::TokenType::kRightParen)) {
    return nullptr;
  }
  return expression;
}

std::vector<std::shared_ptr<ast::Expression>> parse_expression_list(
    Reader& reader, lexer::TokenType end) {
  std::vector<std::shared_ptr<ast::Expression>> expressions;
  if (reader.peek_token_is(end)) {
    reader.next_token();
    return expressions;
  }
  reader.next_token();
  expressions.push_back(parse_expression(reader, Precedence::kLowest));
  while (reader.peek_token_is(lexer::TokenType::kComma)) {
    reader.next_token();
    reader.next_token();
    expressions.push_back(parse_expression(reader, Precedence::kLowest));
  }
  if (!reader.expect_peek(end)) {
    return {};
  }
  return expressions;
}

std::vector<std::shared_ptr<ast::Identifier>> parse_function_parameters(
    Reader& reader) {
  std::vector<std::shared_ptr<ast::Identifier>> parameters;
  if (reader.peek_token_is(lexer::TokenType::kRightParen)) {
    reader.next_token();
    return parameters;
  }
  reader.next_token();
  parameters.push_back(
      std::make_unique<ast::Identifier>(reader.current_token().literal()));
  while (reader.peek_token_is(lexer::TokenType::kComma)) {
    reader.next_token();
    reader.next_token();
    parameters.push_back(
        std::make_unique<ast::Identifier>(reader.current_token().literal()));
  }
  if (!reader.expect_peek(lexer::TokenType::kRightParen)) {
    return {};
  }
  return parameters;
}

}  // namespace monkey::parser