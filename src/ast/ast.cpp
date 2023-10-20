#include <fmt/core.h>
#include <monkey/ast/ast.h>
#include <monkey/ast/stmt.h>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace monkey::ast {

std::string to_string(NodeType type) {
  switch (type) {
    case NodeType::kProgram:
      return "Program";
    case NodeType::kLetStatement:
      return "LetStatement";
    case NodeType::kReturnStatement:
      return "ReturnStatement";
    case NodeType::kExpressionStatement:
      return "ExpressionStatement";
    case NodeType::kBlockStatement:
      return "BlockStatement";

    case NodeType::kIdentifier:
      return "Identifier";
    case NodeType::kIntegerLiteral:
      return "IntegerLiteral";
    case NodeType::kBooleanLiteral:
      return "BooleanLiteral";
    case NodeType::kFunctionLiteral:
      return "FunctionLiteral";
    case NodeType::kStringLiteral:
      return "StringLiteral";
    case NodeType::kArrayLiteral:
      return "ArrayLiteral";
    case NodeType::kHashLiteral:
      return "HashLiteral";

    case NodeType::kPrefixExpression:
      return "PrefixExpression";
    case NodeType::kInfixExpression:
      return "InfixExpression";
    case NodeType::kIfExpression:
      return "IfExpression";
    case NodeType::kCallExpression:
      return "CallExpression";
    case NodeType::kIndexExpression:
      return "IndexExpression";
    default:
      return "Unknown";
  }
}

std::string to_string(const Node& node) { return node.to_string(); }

std::string to_string(const Node&& node) { return node.to_string(); }

Program::Program(std::vector<std::unique_ptr<Statement>> statements)
    : statements_(std::move(statements)) {}

std::string Program::to_string() const {
  std::string statements;
  for (const auto& statement : statements_) {
    if (statements.empty()) {
      statements = statement->to_string();
    } else {
      statements = fmt::format("{}\n{}", statements, statement->to_string());
    }
  }
  return statements;
}

bool Program::operator==(const Node& other) const {
  if (other.type() != NodeType::kProgram) {
    return false;
  }
  const auto& other_program = dynamic_cast<const Program&>(other);
  if (statements_.size() != other_program.statements_.size()) {
    return false;
  }
  return std::ranges::equal(
      statements_, other_program.statements_,
      [](const auto& lhs, const auto& rhs) { return *lhs == *rhs; });
}

bool Program::operator==(const Node&& other) const { return *this == other; }

bool Program::operator!=(const Node& other) const { return !(*this == other); }

bool Program::operator!=(const Node&& other) const { return *this != other; }

}  // namespace monkey::ast