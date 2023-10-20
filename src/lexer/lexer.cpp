#include <monkey/lexer/lexer.h>
#include <monkey/lexer/token.h>

#include <cctype>
#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <utility>

namespace monkey::lexer {

Lexer::Lexer(std::string input) : input_(std::move(input)) {}

Lexer::Iterator Lexer::begin() const {
  return Iterator(input_.begin(), input_.end());
}

Lexer::Iterator Lexer::end() const {
  return Iterator(input_.end(), input_.end());
}

Lexer::Iterator Lexer::cbegin() const {
  return Iterator(input_.begin(), input_.end());
}

Lexer::Iterator Lexer::cend() const {
  return Iterator(input_.end(), input_.end());
}

Lexer::Iterator::Iterator(std::string::const_iterator start_location,
                          std::string::const_iterator end_location)
    : impl_(std::make_unique<IteratorImpl>(start_location, end_location)) {}

Lexer::Iterator::Iterator(const Iterator &other)
    : impl_(std::make_unique<IteratorImpl>(*other.impl_)) {}

Lexer::Iterator &Lexer::Iterator::operator=(const Iterator &other) {
  if (this != &other) {
    impl_ = std::make_unique<IteratorImpl>(*other.impl_);
  }
  return *this;
}

Lexer::Iterator &Lexer::Iterator::operator++() {
  impl_->next_token();
  return *this;
}

Lexer::Iterator Lexer::Iterator::operator++(int) {
  auto copy = *this;
  ++(*this);
  return copy;
}

bool Lexer::Iterator::operator==(const Iterator &rhs) const {
  return *impl_ == *rhs.impl_;
}

bool Lexer::Iterator::operator!=(const Iterator &rhs) const {
  return !(*this == rhs);
}

Token Lexer::Iterator::operator*() const {
  auto impl_copy = *impl_;
  return impl_copy.next_token();
}

Token Lexer::Iterator::operator->() const { return **this; }

Lexer::Iterator::IteratorImpl::IteratorImpl(
    std::string::const_iterator start_location,
    std::string::const_iterator end_location)
    : current_location_(start_location),
      peek_location_(start_location),
      end_location_(end_location) {
  read_char();
}

bool Lexer::Iterator::IteratorImpl::operator==(const IteratorImpl &rhs) const {
  return current_location_ == rhs.current_location_ &&
         peek_location_ == rhs.peek_location_ &&
         end_location_ == rhs.end_location_;
}

bool Lexer::Iterator::IteratorImpl::operator!=(const IteratorImpl &rhs) const {
  return !(*this == rhs);
}

Token Lexer::Iterator::IteratorImpl::next_token() {
  auto token = Token(TokenType::kIllegal, std::string());
  skip_whitespace();
  const char ch = current_char();
  switch (ch) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '<':
    case '>':
    case ',':
    case ';':
    case ':':
    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']':
    case '\0':
      token = Token(kSingleCharTokens.at(ch), std::string(1, ch));
      break;

    case '=':
      if (peek_char() == '=') {
        read_char();
        token = Token(TokenType::kEqual, "==");
      } else {
        token = Token(TokenType::kAssign, "=");
      }
      break;

    case '!':
      if (peek_char() == '=') {
        read_char();
        token = Token(TokenType::kNotEqual, "!=");
      } else {
        token = Token(TokenType::kBang, "!");
      }
      break;

    case '"':
      token = Token(TokenType::kString, read_string());
      break;

    default:
      if (std::isalpha(ch)) {
        auto identifier = read_identifier();
        if (kKeywords.find(identifier) != kKeywords.end()) {
          const auto keyword = kKeywords.at(identifier);
          return Token(keyword, std::move(identifier));
        }
        return Token(TokenType::kIdentifer, std::move(identifier));
      } else if (std::isdigit(ch)) {
        return Token(TokenType::kInteger, read_integer());
      }
  }
  read_char();
  return token;
}

char Lexer::Iterator::IteratorImpl::current_char() const {
  if (current_location_ == end_location_) {
    return '\0';
  }
  return *current_location_;
}

char Lexer::Iterator::IteratorImpl::peek_char() const {
  if (peek_location_ == end_location_) {
    return '\0';
  }
  return *peek_location_;
}

void Lexer::Iterator::IteratorImpl::read_char() {
  current_location_ = peek_location_;
  if (peek_location_ != end_location_) {
    ++peek_location_;
  }
}

void Lexer::Iterator::IteratorImpl::skip_whitespace() {
  while (current_char() == ' ' || current_char() == '\t' ||
         current_char() == '\n' || current_char() == '\r') {
    read_char();
  }
}

std::string Lexer::Iterator::IteratorImpl::read_identifier() {
  const auto start = current_location_;
  while (std::isalpha(current_char()) || current_char() == '_') {
    read_char();
  }
  return std::string(
      &*start, static_cast<size_t>(std::distance(start, current_location_)));
}

std::string Lexer::Iterator::IteratorImpl::read_integer() {
  const auto start = current_location_;
  while (std::isdigit(current_char())) {
    read_char();
  }
  return std::string(
      &*start, static_cast<size_t>(std::distance(start, current_location_)));
}

std::string Lexer::Iterator::IteratorImpl::read_string() {
  read_char();
  const auto start = current_location_;
  while (current_char() != '"') {
    read_char();
  }
  return std::string(
      &*start, static_cast<size_t>(std::distance(start, current_location_)));
}

}  // namespace monkey::lexer