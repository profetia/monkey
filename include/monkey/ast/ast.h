#ifndef MONKEY_AST_AST_H
#define MONKEY_AST_AST_H

#include <memory>
#include <string>
#include <vector>

namespace monkey::ast {

enum class NodeType {
  kProgram,

  kLetStatement,
  kReturnStatement,
  kExpressionStatement,
  kBlockStatement,

  kIdentifier,
  kIntegerLiteral,
  kBooleanLiteral,
  kFunctionLiteral,
  kStringLiteral,
  kArrayLiteral,
  kHashLiteral,
  kMacroLiteral,

  kPrefixExpression,
  kInfixExpression,
  kIfExpression,
  kCallExpression,
  kIndexExpression,
};

std::string to_string(NodeType type);

class Node {
 public:
  Node() = default;
  Node(const Node&) = default;
  Node(Node&&) = default;
  Node& operator=(const Node&) = default;
  Node& operator=(Node&&) = default;
  virtual ~Node() = default;

  [[nodiscard]] virtual NodeType type() const = 0;

  [[nodiscard]] virtual std::string to_string() const = 0;

  virtual bool operator==(const Node& other) const = 0;
  virtual bool operator!=(const Node& other) const = 0;

  friend std::string to_string(const Node& node);
};

class Statement;
class LetStatement;
class ReturnStatement;
class ExpressionStatement;
class BlockStatement;

class Expression;
class Identifier;
class IntegerLiteral;
class BooleanLiteral;
class StringLiteral;
class ArrayLiteral;
class HashLiteral;
class PrefixExpression;
class InfixExpression;
class IfExpression;
class FunctionLiteral;
class CallExpression;
class IndexExpression;

class Program : public Node {
 public:
  explicit Program(std::vector<std::shared_ptr<Statement>> statements);

  [[nodiscard]] NodeType type() const override { return NodeType::kProgram; }
  [[nodiscard]] const std::vector<std::shared_ptr<Statement>>& statements()
      const {
    return statements_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::vector<std::shared_ptr<Statement>> statements_;
};

}  // namespace monkey::ast

#endif  // MONKEY_AST_AST_H