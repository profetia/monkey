#include <fmt/core.h>
#include <monkey/ast/expr.h>
#include <monkey/ast/stmt.h>

#include <ranges>

namespace monkey::ast {

LetStatement::LetStatement(std::unique_ptr<Identifier> name,
                           std::unique_ptr<Expression> value)
    : name_(std::move(name)), value_(std::move(value)) {}

std::string LetStatement::to_string() const {
  return fmt::format("let {} = {};", name_->to_string(), value_->to_string());
}

bool LetStatement::operator==(const Node& other) const {
  if (other.type() != NodeType::kLetStatement) {
    return false;
  }
  const auto& other_let = dynamic_cast<const LetStatement&>(other);
  return name_->operator==(*other_let.name_) &&
         value_->operator==(*other_let.value_);
}

bool LetStatement::operator==(const Node&& other) const {
  return *this == other;
}

bool LetStatement::operator!=(const Node& other) const {
  return !(*this == other);
}

bool LetStatement::operator!=(const Node&& other) const {
  return *this != other;
}

ReturnStatement::ReturnStatement(std::unique_ptr<Expression> return_value)
    : return_value_(std::move(return_value)) {}

std::string ReturnStatement::to_string() const {
  return fmt::format("return {};", return_value_->to_string());
}

bool ReturnStatement::operator==(const Node& other) const {
  if (other.type() != NodeType::kReturnStatement) {
    return false;
  }
  const auto& other_return = dynamic_cast<const ReturnStatement&>(other);
  return *return_value_ == *other_return.return_value_;
}

bool ReturnStatement::operator==(const Node&& other) const {
  return *this == other;
}

bool ReturnStatement::operator!=(const Node& other) const {
  return !(*this == other);
}

bool ReturnStatement::operator!=(const Node&& other) const {
  return *this != other;
}

ExpressionStatement::ExpressionStatement(std::unique_ptr<Expression> expression)
    : expression_(std::move(expression)) {}

std::string ExpressionStatement::to_string() const {
  return expression_->to_string();
}

bool ExpressionStatement::operator==(const Node& other) const {
  if (other.type() != NodeType::kExpressionStatement) {
    return false;
  }
  const auto& other_expression =
      dynamic_cast<const ExpressionStatement&>(other);
  return *expression_ == *other_expression.expression_;
}

bool ExpressionStatement::operator==(const Node&& other) const {
  return *this == other;
}

bool ExpressionStatement::operator!=(const Node& other) const {
  return !(*this == other);
}

bool ExpressionStatement::operator!=(const Node&& other) const {
  return *this != other;
}

BlockStatement::BlockStatement(
    std::vector<std::unique_ptr<Statement>> statements)
    : statements_(std::move(statements)) {}

std::string BlockStatement::to_string() const {
  std::string statements;
  for (const auto& statement : statements_) {
    statements = fmt::format("{}\n{}", statements, statement->to_string());
  }
  return fmt::format("{}", statements);
}

bool BlockStatement::operator==(const Node& other) const {
  if (other.type() != NodeType::kBlockStatement) {
    return false;
  }
  const auto& other_block = dynamic_cast<const BlockStatement&>(other);
  return std::ranges::equal(
      statements_, other_block.statements_,
      [](const auto& lhs, const auto& rhs) { return *lhs == *rhs; });
}

bool BlockStatement::operator==(const Node&& other) const {
  return *this == other;
}

bool BlockStatement::operator!=(const Node& other) const {
  return !(*this == other);
}

bool BlockStatement::operator!=(const Node&& other) const {
  return *this != other;
}

}  // namespace monkey::ast
