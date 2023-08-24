#ifndef MONKEY_LEXER_TOKEN_H_
#define MONKEY_LEXER_TOKEN_H_

#include <string>
#include <unordered_map>

namespace monkey::lexer {

enum class TokenType {
  kIllegal,
  kEOF,

  // Identifiers + literals
  kIdentifer,
  kInteger,
  kBoolean,
  kString,

  // Operators
  kAssign,
  kPlus,
  kMinus,
  kBang,
  kAsterisk,
  kSlash,
  kLessThan,
  kGreaterThan,
  kEqual,
  kNotEqual,

  // Delimiters
  kComma,
  kSemicolon,
  kColon,

  kLeftParen,
  kRightParen,
  kLeftBrace,
  kRightBrace,
  kLeftBracket,
  kRightBracket,

  // Keywords
  kFunction,
  kLet,
  kTrue,
  kFalse,
  kIf,
  kElse,
  kReturn,
  kMacro,
};  // enum class TokenType

std::string to_string(TokenType type);

std::string to_operator(TokenType type);

class Token {
 public:
  Token(TokenType type, std::string literal);

  bool operator==(const Token& rhs) const;
  bool operator==(const Token&& rhs) const;
  bool operator!=(const Token& rhs) const;
  bool operator!=(const Token&& rhs) const;

  [[nodiscard]] TokenType type() const { return type_; }
  [[nodiscard]] std::string literal() const { return literal_; }

  friend std::string to_string(const Token& token);
  friend std::string to_string(const Token&& token);

 private:
  TokenType type_;
  std::string literal_;
};  // class Token

const std::unordered_map<std::string, TokenType> kKeywords = {
    {"fn", TokenType::kFunction},   {"let", TokenType::kLet},
    {"true", TokenType::kTrue},     {"false", TokenType::kFalse},
    {"if", TokenType::kIf},         {"else", TokenType::kElse},
    {"return", TokenType::kReturn}, {"macro", TokenType::kMacro}};

const std::unordered_map<char, TokenType> kSingleCharTokens = {
    {'=', TokenType::kAssign},       {'+', TokenType::kPlus},
    {'-', TokenType::kMinus},        {'!', TokenType::kBang},
    {'*', TokenType::kAsterisk},     {'/', TokenType::kSlash},
    {'<', TokenType::kLessThan},     {'>', TokenType::kGreaterThan},
    {',', TokenType::kComma},        {';', TokenType::kSemicolon},
    {':', TokenType::kColon},        {'(', TokenType::kLeftParen},
    {')', TokenType::kRightParen},   {'{', TokenType::kLeftBrace},
    {'}', TokenType::kRightBrace},   {'[', TokenType::kLeftBracket},
    {']', TokenType::kRightBracket}, {'\0', TokenType::kEOF}};
}  // namespace monkey::lexer

#endif  // MONKEY_LEXER_TOKEN_H_