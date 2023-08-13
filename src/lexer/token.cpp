#include <fmt/core.h>
#include <monkey/lexer/token.h>

namespace monkey::lexer {

std::string to_string(TokenType type) {
  switch (type) {
    case TokenType::kIllegal:
      return "ILLEGAL";
    case TokenType::kEOF:
      return "EOF";

    // Identifiers + literals
    case TokenType::kIdentifer:
      return "IDENTIFIER";
    case TokenType::kInteger:
      return "INTEGER";
    case TokenType::kBoolean:
      return "BOOLEAN";
    case TokenType::kString:
      return "STRING";

    // Operators
    case TokenType::kAssign:
      return "ASSIGN";
    case TokenType::kPlus:
      return "PLUS";
    case TokenType::kMinus:
      return "MINUS";
    case TokenType::kBang:
      return "BANG";
    case TokenType::kAsterisk:
      return "ASTERISK";
    case TokenType::kSlash:
      return "SLASH";
    case TokenType::kLessThan:
      return "LESS_THAN";
    case TokenType::kGreaterThan:
      return "GREATER_THAN";
    case TokenType::kEqual:
      return "EQUAL";
    case TokenType::kNotEqual:
      return "NOT_EQUAL";

    // Delimiters
    case TokenType::kComma:
      return "COMMA";
    case TokenType::kSemicolon:
      return "SEMICOLON";
    case TokenType::kColon:
      return "COLON";

    case TokenType::kLeftParen:
      return "LEFT_PAREN";
    case TokenType::kRightParen:
      return "RIGHT_PAREN";
    case TokenType::kLeftBrace:
      return "LEFT_BRACE";
    case TokenType::kRightBrace:
      return "RIGHT_BRACE";
    case TokenType::kLeftBracket:
      return "LEFT_BRACKET";
    case TokenType::kRightBracket:
      return "RIGHT_BRACKET";

    // Keywords
    case TokenType::kFunction:
      return "FUNCTION";
    case TokenType::kLet:
      return "LET";

    default:
      return "UNKNOWN";
  }
}

Token::Token(TokenType type, std::string literal)
    : type_(type), literal_(std::move(literal)) {}

bool Token::operator==(const Token &rhs) const {
  return type_ == rhs.type_ && literal_ == rhs.literal_;
}

bool Token::operator==(const Token &&rhs) const { return *this == rhs; }

bool Token::operator!=(const Token &rhs) const { return !(*this == rhs); }

bool Token::operator!=(const Token &&rhs) const { return *this != rhs; }

std::string to_string(const Token &token) {
  return fmt::format("Token({}: \"{}\")", to_string(token.type()),
                     token.literal());
}

std::string to_string(const Token &&token) { return to_string(token); }

}  // namespace monkey::lexer