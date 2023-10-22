#include <fmt/core.h>
#include <monkey/lexer/token.h>
#include <monkey/parser/error.h>

#include <string>
#include <utility>

namespace monkey::parser {

ParserError::ParserError(const std::string& message)
    : ParserError(std::string(message)) {}

ParserError::ParserError(std::string&& message)
    : message_(fmt::format("ParseError: {}", std::move(message))) {}

PeekTokenError::PeekTokenError(lexer::TokenType expect, lexer::TokenType got)
    : ParserError(fmt::format("expect next token to be {}, got {} instead",
                              lexer::to_string(expect),
                              lexer::to_string(got))) {}

HandlerNotFoundError::HandlerNotFoundError(lexer::TokenType type)
    : ParserError(fmt::format("no parse handler for {} found",
                              lexer::to_string(type))) {}

InvalidIntegerError::InvalidIntegerError(const std::string& literal)
    : InvalidIntegerError(std::string(literal)) {}

InvalidIntegerError::InvalidIntegerError(std::string&& literal)
    : ParserError(
          fmt::format("could not parse {} as integer", std::move(literal))) {}

}  // namespace monkey::parser