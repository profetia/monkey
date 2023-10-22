#ifndef MONKEY_PARSER_EXPR_H_
#define MONKEY_PARSER_EXPR_H_

#include <monkey/ast/expr.h>
#include <monkey/lexer/token.h>
#include <monkey/parser/reader.h>

#include <functional>
#include <unordered_map>
#include <variant>

namespace monkey::parser {

enum class Precedence {
  kLowest,
  kEquality,
  kComparison,
  kSum,
  kProduct,
  kPrefix,
  kCall,
  kIndex
};

const std::unordered_map<lexer::TokenType, Precedence> kPrecedences = {
    {lexer::TokenType::kEqual, Precedence::kEquality},
    {lexer::TokenType::kNotEqual, Precedence::kEquality},
    {lexer::TokenType::kLessThan, Precedence::kComparison},
    {lexer::TokenType::kGreaterThan, Precedence::kComparison},
    {lexer::TokenType::kPlus, Precedence::kSum},
    {lexer::TokenType::kMinus, Precedence::kSum},
    {lexer::TokenType::kSlash, Precedence::kProduct},
    {lexer::TokenType::kAsterisk, Precedence::kProduct},
    {lexer::TokenType::kLeftParen, Precedence::kCall},
    {lexer::TokenType::kLeftBracket, Precedence::kIndex}};

Precedence get_precedence(lexer::TokenType type);

std::shared_ptr<ast::Expression> parse_expression(Reader& reader,
                                                  Precedence precedence);

std::shared_ptr<ast::Identifier> parse_identifier(Reader& reader);

std::shared_ptr<ast::IntegerLiteral> parse_integer_literal(Reader& reader);

std::shared_ptr<ast::BooleanLiteral> parse_boolean_literal(Reader& reader);

std::shared_ptr<ast::StringLiteral> parse_string_literal(Reader& reader);

std::shared_ptr<ast::ArrayLiteral> parse_array_literal(Reader& reader);

std::shared_ptr<ast::HashLiteral> parse_hash_literal(Reader& reader);

std::shared_ptr<ast::FunctionLiteral> parse_function_literal(Reader& reader);

std::shared_ptr<ast::MacroLiteral> parse_macro_literal(Reader& reader);

std::shared_ptr<ast::PrefixExpression> parse_prefix_expression(Reader& reader);

std::shared_ptr<ast::InfixExpression> parse_infix_expression(
    Reader& reader, std::shared_ptr<ast::Expression> left);

std::shared_ptr<ast::IndexExpression> parse_index_expression(
    Reader& reader, std::shared_ptr<ast::Expression> left);

std::shared_ptr<ast::IfExpression> parse_if_expression(Reader& reader);

std::shared_ptr<ast::CallExpression> parse_call_expression(
    Reader& reader, std::shared_ptr<ast::Expression> function);

std::shared_ptr<ast::Expression> parse_grouped_expression(Reader& reader);

std::vector<std::shared_ptr<ast::Expression>> parse_expression_list(
    Reader& reader, lexer::TokenType end);

std::vector<std::shared_ptr<ast::Identifier>> parse_function_parameters(
    Reader& reader);

using PrefixHandler = std::function<std::shared_ptr<ast::Expression>(Reader&)>;
using InfixHandler = std::function<std::shared_ptr<ast::Expression>(
    Reader&, std::shared_ptr<ast::Expression>)>;

const std::unordered_map<lexer::TokenType, PrefixHandler> kPrefixHandlers = {
    {lexer::TokenType::kIdentifer, parse_identifier},
    {lexer::TokenType::kInteger, parse_integer_literal},
    {lexer::TokenType::kTrue, parse_boolean_literal},
    {lexer::TokenType::kFalse, parse_boolean_literal},
    {lexer::TokenType::kString, parse_string_literal},
    {lexer::TokenType::kLeftBracket, parse_array_literal},
    {lexer::TokenType::kLeftBrace, parse_hash_literal},
    {lexer::TokenType::kFunction, parse_function_literal},
    {lexer::TokenType::kMacro, parse_macro_literal},
    {lexer::TokenType::kBang, parse_prefix_expression},
    {lexer::TokenType::kMinus, parse_prefix_expression},
    {lexer::TokenType::kLeftParen, parse_grouped_expression},
    {lexer::TokenType::kIf, parse_if_expression}};
const std::unordered_map<lexer::TokenType, InfixHandler> kInfixHandlers = {
    {lexer::TokenType::kPlus, parse_infix_expression},
    {lexer::TokenType::kMinus, parse_infix_expression},
    {lexer::TokenType::kSlash, parse_infix_expression},
    {lexer::TokenType::kAsterisk, parse_infix_expression},
    {lexer::TokenType::kEqual, parse_infix_expression},
    {lexer::TokenType::kNotEqual, parse_infix_expression},
    {lexer::TokenType::kLessThan, parse_infix_expression},
    {lexer::TokenType::kGreaterThan, parse_infix_expression},
    {lexer::TokenType::kLeftParen, parse_call_expression},
    {lexer::TokenType::kLeftBracket, parse_index_expression}};

}  // namespace monkey::parser

#endif  // MONKEY_PARSER_EXPR_H_