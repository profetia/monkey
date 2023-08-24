#include <fmt/core.h>
#include <monkey/ast/expr.h>
#include <monkey/ast/stmt.h>

#include <algorithm>
#include <ranges>

namespace monkey::ast {

Identifier::Identifier(std::string name) : name_(std::move(name)) {}

std::string Identifier::to_string() const { return fmt::format("{}", name_); }

bool Identifier::operator==(const Node& other) const {
  if (other.type() != NodeType::kIdentifier) {
    return false;
  }
  const auto& other_identifier = dynamic_cast<const Identifier&>(other);
  return name_ == other_identifier.name_;
}

bool Identifier::operator==(const Node&& other) const { return *this == other; }

bool Identifier::operator!=(const Node& other) const {
  return !(*this == other);
}

bool Identifier::operator!=(const Node&& other) const { return *this != other; }

IntegerLiteral::IntegerLiteral(int64_t value) : value_(value) {}

std::string IntegerLiteral::to_string() const {
  return fmt::format("{}", value_);
}

bool IntegerLiteral::operator==(const Node& other) const {
  if (other.type() != NodeType::kIntegerLiteral) {
    return false;
  }
  const auto& other_integer_literal =
      dynamic_cast<const IntegerLiteral&>(other);
  return value_ == other_integer_literal.value_;
}

bool IntegerLiteral::operator==(const Node&& other) const {
  return *this == other;
}

bool IntegerLiteral::operator!=(const Node& other) const {
  return !(*this == other);
}

bool IntegerLiteral::operator!=(const Node&& other) const {
  return *this != other;
}

BooleanLiteral::BooleanLiteral(bool value) : value_(value) {}

std::string BooleanLiteral::to_string() const {
  return fmt::format("{}", value_);
}

bool BooleanLiteral::operator==(const Node& other) const {
  if (other.type() != NodeType::kBooleanLiteral) {
    return false;
  }
  const auto& other_boolean_literal =
      dynamic_cast<const BooleanLiteral&>(other);
  return value_ == other_boolean_literal.value_;
}

bool BooleanLiteral::operator==(const Node&& other) const {
  return *this == other;
}

bool BooleanLiteral::operator!=(const Node& other) const {
  return !(*this == other);
}

bool BooleanLiteral::operator!=(const Node&& other) const {
  return *this != other;
}

FunctionLiteral::FunctionLiteral(
    std::vector<std::unique_ptr<Identifier>> parameters,
    std::unique_ptr<BlockStatement> body)
    : parameters_(std::move(parameters)), body_(std::move(body)) {}

std::string FunctionLiteral::to_string() const {
  std::string parameters;
  for (const auto& parameter : parameters_) {
    parameters = fmt::format("{}, {}", parameters, parameter->to_string());
  }
  return fmt::format("fn({}) {{{}}}", parameters, body_->to_string());
}

bool FunctionLiteral::operator==(const Node& other) const {
  if (other.type() != NodeType::kFunctionLiteral) {
    return false;
  }
  const auto& other_function_literal =
      dynamic_cast<const FunctionLiteral&>(other);
  if (parameters_.size() != other_function_literal.parameters_.size()) {
    return false;
  }

  if (!std::ranges::equal(parameters_, other_function_literal.parameters_,
                          [](const auto& lhs, const auto& rhs) {
                            return lhs->operator==(*rhs);
                          })) {
    return false;
  }

  return body_->operator==(*other_function_literal.body_);
}

bool FunctionLiteral::operator==(const Node&& other) const {
  return *this == other;
}

bool FunctionLiteral::operator!=(const Node& other) const {
  return !(*this == other);
}

bool FunctionLiteral::operator!=(const Node&& other) const {
  return *this != other;
}

StringLiteral::StringLiteral(std::string value) : value_(std::move(value)) {}

std::string StringLiteral::to_string() const {
  return fmt::format("{}", value_);
}

bool StringLiteral::operator==(const Node& other) const {
  if (other.type() != NodeType::kStringLiteral) {
    return false;
  }
  const auto& other_string_literal = dynamic_cast<const StringLiteral&>(other);
  return value_ == other_string_literal.value_;
}

bool StringLiteral::operator==(const Node&& other) const {
  return *this == other;
}

bool StringLiteral::operator!=(const Node& other) const {
  return !(*this == other);
}

bool StringLiteral::operator!=(const Node&& other) const {
  return *this != other;
}

ArrayLiteral::ArrayLiteral(std::vector<std::unique_ptr<Expression>> elements)
    : elements_(std::move(elements)) {}

std::string ArrayLiteral::to_string() const {
  std::string elements;
  for (const auto& element : elements_) {
    if (elements.empty()) {
      elements = element->to_string();
    } else {
      elements = fmt::format("{}, {}", elements, element->to_string());
    }
  }
  return fmt::format("[{}]", elements);
}

bool ArrayLiteral::operator==(const Node& other) const {
  if (other.type() != NodeType::kArrayLiteral) {
    return false;
  }
  const auto& other_array_literal = dynamic_cast<const ArrayLiteral&>(other);
  if (elements_.size() != other_array_literal.elements_.size()) {
    return false;
  }

  return std::ranges::equal(
      elements_, other_array_literal.elements_,
      [](const auto& lhs, const auto& rhs) { return *lhs == *rhs; });
}

bool ArrayLiteral::operator==(const Node&& other) const {
  return *this == other;
}

bool ArrayLiteral::operator!=(const Node& other) const {
  return !(*this == other);
}

bool ArrayLiteral::operator!=(const Node&& other) const {
  return *this != other;
}

HashLiteral::HashLiteral(
    std::unordered_map<std::unique_ptr<Expression>, std::unique_ptr<Expression>>
        pairs)
    : pairs_(std::move(pairs)) {}

std::string HashLiteral::to_string() const {
  std::string pairs;
  for (const auto& [key, value] : pairs_) {
    if (pairs.empty()) {
      pairs = fmt::format("{}: {}", key->to_string(), value->to_string());
    } else {
      pairs = fmt::format("{}, {}: {}", pairs, key->to_string(),
                          value->to_string());
    }
  }
  return fmt::format("{{{}}}", pairs);
}

bool HashLiteral::operator==(const Node& other) const {
  if (other.type() != NodeType::kHashLiteral) {
    return false;
  }
  const auto& other_hash_literal = dynamic_cast<const HashLiteral&>(other);
  if (pairs_.size() != other_hash_literal.pairs_.size()) {
    return false;
  }
  return std::ranges::all_of(pairs_, [&other_hash_literal](const auto& pair) {
    return std::ranges::any_of(other_hash_literal.pairs_,
                               [&pair](const auto& other_pair) {
                                 return *pair.first == *other_pair.first &&
                                        *pair.second == *other_pair.second;
                               });
  });
  return true;
}

bool HashLiteral::operator==(const Node&& other) const {
  return *this == other;
}

bool HashLiteral::operator!=(const Node& other) const {
  return !(*this == other);
}

bool HashLiteral::operator!=(const Node&& other) const {
  return *this != other;
}

MacroLiteral::MacroLiteral(std::vector<std::unique_ptr<Identifier>> parameters,
                           std::unique_ptr<BlockStatement> body)
    : parameters_(std::move(parameters)), body_(std::move(body)) {}

std::string MacroLiteral::to_string() const {
  std::string parameters;
  for (const auto& parameter : parameters_) {
    parameters = fmt::format("{}, {}", parameters, parameter->to_string());
  }
  return fmt::format("macro({}) {{{}}}", parameters, body_->to_string());
}

bool MacroLiteral::operator==(const Node& other) const {
  if (other.type() != NodeType::kMacroLiteral) {
    return false;
  }
  const auto& other_macro_literal = dynamic_cast<const MacroLiteral&>(other);
  if (parameters_.size() != other_macro_literal.parameters_.size()) {
    return false;
  }

  if (!std::ranges::equal(parameters_, other_macro_literal.parameters_,
                          [](const auto& lhs, const auto& rhs) {
                            return lhs->operator==(*rhs);
                          })) {
    return false;
  }

  return body_->operator==(*other_macro_literal.body_);
}

bool MacroLiteral::operator==(const Node&& other) const {
  return *this == other;
}

bool MacroLiteral::operator!=(const Node& other) const {
  return !(*this == other);
}

bool MacroLiteral::operator!=(const Node&& other) const {
  return *this != other;
}

PrefixExpression::PrefixExpression(lexer::TokenType op,
                                   std::unique_ptr<Expression> right)
    : op_(op), right_(std::move(right)) {}

std::string PrefixExpression::to_string() const {
  return fmt::format("({}{})", lexer::to_operator(op_), right_->to_string());
}

bool PrefixExpression::operator==(const Node& other) const {
  if (other.type() != NodeType::kPrefixExpression) {
    return false;
  }
  const auto& other_prefix_expression =
      dynamic_cast<const PrefixExpression&>(other);
  return op_ == other_prefix_expression.op_ &&
         *right_ == *other_prefix_expression.right_;
}

bool PrefixExpression::operator==(const Node&& other) const {
  return *this == other;
}

bool PrefixExpression::operator!=(const Node& other) const {
  return !(*this == other);
}

bool PrefixExpression::operator!=(const Node&& other) const {
  return *this != other;
}

InfixExpression::InfixExpression(std::unique_ptr<Expression> left,
                                 lexer::TokenType op,
                                 std::unique_ptr<Expression> right)
    : left_(std::move(left)), op_(op), right_(std::move(right)) {}

std::string InfixExpression::to_string() const {
  return fmt::format("({} {} {})", left_->to_string(), lexer::to_operator(op_),
                     right_->to_string());
}

bool InfixExpression::operator==(const Node& other) const {
  if (other.type() != NodeType::kInfixExpression) {
    return false;
  }
  const auto& other_infix_expression =
      dynamic_cast<const InfixExpression&>(other);
  return *left_ == *other_infix_expression.left_ &&
         op_ == other_infix_expression.op_ &&
         *right_ == *other_infix_expression.right_;
}

bool InfixExpression::operator==(const Node&& other) const {
  return *this == other;
}

bool InfixExpression::operator!=(const Node& other) const {
  return !(*this == other);
}

bool InfixExpression::operator!=(const Node&& other) const {
  return *this != other;
}

IndexExpression::IndexExpression(std::unique_ptr<Expression> left,
                                 std::unique_ptr<Expression> index)
    : left_(std::move(left)), index_(std::move(index)) {}

std::string IndexExpression::to_string() const {
  return fmt::format("({}[{}])", left_->to_string(), index_->to_string());
}

bool IndexExpression::operator==(const Node& other) const {
  if (other.type() != NodeType::kIndexExpression) {
    return false;
  }
  const auto& other_index_expression =
      dynamic_cast<const IndexExpression&>(other);
  return *left_ == *other_index_expression.left_ &&
         *index_ == *other_index_expression.index_;
}

bool IndexExpression::operator==(const Node&& other) const {
  return *this == other;
}

bool IndexExpression::operator!=(const Node& other) const {
  return !(*this == other);
}

bool IndexExpression::operator!=(const Node&& other) const {
  return *this != other;
}

IfExpression::IfExpression(std::unique_ptr<Expression> condition,
                           std::unique_ptr<BlockStatement> consequence,
                           std::unique_ptr<BlockStatement> alternative)
    : condition_(std::move(condition)),
      consequence_(std::move(consequence)),
      alternative_(std::move(alternative)) {}

std::string IfExpression::to_string() const {
  if (alternative_) {
    return fmt::format("if ({}) {{{}}} else {{{}}}", condition_->to_string(),
                       consequence_->to_string(), alternative_->to_string());
  }
  return fmt::format("if ({}) {{{}}}", condition_->to_string(),
                     consequence_->to_string());
}

bool IfExpression::operator==(const Node& other) const {
  if (other.type() != NodeType::kIfExpression) {
    return false;
  }
  const auto& other_if_expression = dynamic_cast<const IfExpression&>(other);
  return *condition_ == *other_if_expression.condition_ &&
         consequence_->operator==(*other_if_expression.consequence_) &&
         alternative_->operator==(*other_if_expression.alternative_);
}

bool IfExpression::operator==(const Node&& other) const {
  return *this == other;
}

bool IfExpression::operator!=(const Node& other) const {
  return !(*this == other);
}

bool IfExpression::operator!=(const Node&& other) const {
  return *this != other;
}

CallExpression::CallExpression(
    std::unique_ptr<Expression> function,
    std::vector<std::unique_ptr<Expression>> arguments)
    : function_(std::move(function)), arguments_(std::move(arguments)) {}

std::string CallExpression::to_string() const {
  std::string args;
  for (const auto& arg : arguments_) {
    if (args.empty()) {
      args = arg->to_string();
    } else {
      args = fmt::format("{}, {}", args, arg->to_string());
    }
  }
  return fmt::format("{}({})", function_->to_string(), args);
}

bool CallExpression::operator==(const Node& other) const {
  if (other.type() != NodeType::kCallExpression) {
    return false;
  }
  const auto& other_call_expression =
      dynamic_cast<const CallExpression&>(other);
  if (arguments_.size() != other_call_expression.arguments_.size()) {
    return false;
  }

  if (!std::ranges::equal(
          arguments_, other_call_expression.arguments_,
          [](const auto& lhs, const auto& rhs) { return *lhs == *rhs; })) {
    return false;
  }

  return *function_ == *other_call_expression.function_;
}

bool CallExpression::operator==(const Node&& other) const {
  return *this == other;
}

bool CallExpression::operator!=(const Node& other) const {
  return !(*this == other);
}

bool CallExpression::operator!=(const Node&& other) const {
  return *this != other;
}

}  // namespace monkey::ast