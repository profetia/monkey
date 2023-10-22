#ifndef MONKEY_AST_EXPR_H
#define MONKEY_AST_EXPR_H

#include <monkey/ast/ast.h>
#include <monkey/lexer/token.h>

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace monkey::ast {

class Expression : public Node {};

class Identifier : public Expression {
 public:
  explicit Identifier(std::string name);

  [[nodiscard]] NodeType type() const override { return NodeType::kIdentifier; }
  [[nodiscard]] std::string name() const { return name_; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::string name_;
};

class IntegerLiteral : public Expression {
 public:
  explicit IntegerLiteral(int64_t value);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kIntegerLiteral;
  }
  [[nodiscard]] int64_t value() const { return value_; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  int64_t value_;
};

class BooleanLiteral : public Expression {
 public:
  explicit BooleanLiteral(bool value);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kBooleanLiteral;
  }
  [[nodiscard]] bool value() const { return value_; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  bool value_;
};

class FunctionLiteral : public Expression {
 public:
  FunctionLiteral(std::vector<std::shared_ptr<Identifier>> parameters,
                  std::shared_ptr<BlockStatement> body);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kFunctionLiteral;
  }
  [[nodiscard]] const std::vector<std::shared_ptr<Identifier>>& parameters()
      const {
    return parameters_;
  }
  [[nodiscard]] const std::shared_ptr<BlockStatement>& body() const {
    return body_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::vector<std::shared_ptr<Identifier>> parameters_;
  std::shared_ptr<BlockStatement> body_;
};

class StringLiteral : public Expression {
 public:
  explicit StringLiteral(std::string value);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kStringLiteral;
  }
  [[nodiscard]] std::string value() const { return value_; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::string value_;
};

class ArrayLiteral : public Expression {
 public:
  explicit ArrayLiteral(std::vector<std::shared_ptr<Expression>> elements);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kArrayLiteral;
  }
  [[nodiscard]] const std::vector<std::shared_ptr<Expression>>& elements()
      const {
    return elements_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::vector<std::shared_ptr<Expression>> elements_;
};

class HashLiteral : public Expression {
 public:
  explicit HashLiteral(std::unordered_map<std::shared_ptr<Expression>,
                                          std::shared_ptr<Expression>>
                           pairs);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kHashLiteral;
  }
  [[nodiscard]] const std::unordered_map<std::shared_ptr<Expression>,
                                         std::shared_ptr<Expression>>&
  pairs() const {
    return pairs_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::unordered_map<std::shared_ptr<Expression>, std::shared_ptr<Expression>>
      pairs_;
};

class PrefixExpression : public Expression {
 public:
  PrefixExpression(lexer::TokenType op, std::shared_ptr<Expression> right);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kPrefixExpression;
  }
  [[nodiscard]] lexer::TokenType op() const { return op_; }
  [[nodiscard]] const std::shared_ptr<Expression>& right() const {
    return right_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  lexer::TokenType op_;
  std::shared_ptr<Expression> right_;
};

class InfixExpression : public Expression {
 public:
  InfixExpression(std::shared_ptr<Expression> left, lexer::TokenType op,
                  std::shared_ptr<Expression> right);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kInfixExpression;
  }
  [[nodiscard]] const std::shared_ptr<Expression>& left() const {
    return left_;
  }
  [[nodiscard]] lexer::TokenType op() const { return op_; }
  [[nodiscard]] const std::shared_ptr<Expression>& right() const {
    return right_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::shared_ptr<Expression> left_;
  lexer::TokenType op_;
  std::shared_ptr<Expression> right_;
};

class IndexExpression : public Expression {
 public:
  IndexExpression(std::shared_ptr<Expression> left,
                  std::shared_ptr<Expression> index);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kIndexExpression;
  }
  [[nodiscard]] const std::shared_ptr<Expression>& left() const {
    return left_;
  }
  [[nodiscard]] const std::shared_ptr<Expression>& index() const {
    return index_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::shared_ptr<Expression> left_;
  std::shared_ptr<Expression> index_;
};

class IfExpression : public Expression {
 public:
  IfExpression(std::shared_ptr<Expression> condition,
               std::shared_ptr<BlockStatement> consequence,
               std::shared_ptr<BlockStatement> alternative);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kIfExpression;
  }
  [[nodiscard]] const std::shared_ptr<Expression>& condition() const {
    return condition_;
  }
  [[nodiscard]] const std::shared_ptr<BlockStatement>& consequence() const {
    return consequence_;
  }
  [[nodiscard]] const std::shared_ptr<BlockStatement>& alternative() const {
    return alternative_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::shared_ptr<Expression> condition_;
  std::shared_ptr<BlockStatement> consequence_;
  std::shared_ptr<BlockStatement> alternative_;
};

class CallExpression : public Expression {
 public:
  CallExpression(std::shared_ptr<Expression> function,
                 std::vector<std::shared_ptr<Expression>> arguments);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kCallExpression;
  }
  [[nodiscard]] const std::shared_ptr<Expression>& function() const {
    return function_;
  }
  [[nodiscard]] const std::vector<std::shared_ptr<Expression>>& arguments()
      const {
    return arguments_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::shared_ptr<Expression> function_;
  std::vector<std::shared_ptr<Expression>> arguments_;
};

}  // namespace monkey::ast

#endif  // MONKEY_AST_EXPR_H