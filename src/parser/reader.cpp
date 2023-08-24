#include <monkey/parser/error.h>
#include <monkey/parser/reader.h>

namespace monkey::parser {

Reader::Reader(lexer::Lexer::Iterator current_location,
               lexer::Lexer::Iterator end_location)
    : current_location_(current_location),
      peek_location_(std::move(current_location)),
      end_location_(std::move(end_location)) {
  next_token();
}

bool Reader ::operator==(const Reader &rhs) const {
  return current_location_ == rhs.current_location_ &&
         peek_location_ == rhs.peek_location_ &&
         end_location_ == rhs.end_location_;
}

bool Reader::operator==(const Reader &&rhs) const { return *this == rhs; }

bool Reader::operator!=(const Reader &rhs) const { return !(*this == rhs); }

bool Reader::operator!=(const Reader &&rhs) const { return *this != rhs; }

void Reader::next_token() {
  current_location_ = peek_location_;
  if (peek_location_ != end_location_) {
    ++peek_location_;
  }
}

lexer::Token Reader::current_token() const { return *current_location_; }

lexer::Token Reader::peek_token() const { return *peek_location_; }

bool Reader::expect_peek(lexer::TokenType type) {
  if (peek_token_is(type)) {
    next_token();
    return true;
  }
  throw PeekTokenError(type, peek_token().type());
}

bool Reader::current_token_is(lexer::TokenType type) const {
  return current_token().type() == type;
}

bool Reader::peek_token_is(lexer::TokenType type) const {
  return peek_token().type() == type;
}

}  // namespace monkey::parser