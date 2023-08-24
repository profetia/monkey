#ifndef MONKEY_PARSER_ERROR_H_
#define MONKEY_PARSER_ERROR_H_

#include <monkey/lexer/lexer.h>

#include <exception>

namespace monkey::parser {

class ParserError : public std::exception {
 public:
  explicit ParserError(const std::string& message);
  explicit ParserError(std::string&& message);

  [[nodiscard]] const char* what() const noexcept override {
    return message_.c_str();
  }

 private:
  std::string message_;
};

class PeekTokenError : public ParserError {
 public:
  PeekTokenError(lexer::TokenType expect, lexer::TokenType got);
};

class HandlerNotFoundError : public ParserError {
 public:
  explicit HandlerNotFoundError(lexer::TokenType type);
};

class InvalidIntegerError : public ParserError {
 public:
  explicit InvalidIntegerError(const std::string& literal);
  explicit InvalidIntegerError(std::string&& literal)
      : InvalidIntegerError(literal) {}
};

}  // namespace monkey::parser

#endif  // MONKEY_PARSER_ERROR_H_