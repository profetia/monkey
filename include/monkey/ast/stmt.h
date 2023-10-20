#ifndef MONKEY_AST_STMT_H
#define MONKEY_AST_STMT_H

#include <monkey/ast/ast.h>

#include <memory>
#include <string>
#include <vector>

namespace monkey::ast {

class Statement : public Node {};

class LetStatement : public Statement {
 public:
  LetStatement(std::unique_ptr<Identifier> name,
               std::unique_ptr<Expression> value);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kLetStatement;
  }
  [[nodiscard]] const std::unique_ptr<Identifier>& name() const {
    return name_;
  }
  [[nodiscard]] const std::unique_ptr<Expression>& value() const {
    return value_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator==(const Node&& other) const override;
  bool operator!=(const Node& other) const override;
  bool operator!=(const Node&& other) const override;

 private:
  std::unique_ptr<Identifier> name_;
  std::unique_ptr<Expression> value_;
};

class ReturnStatement : public Statement {
 public:
  explicit ReturnStatement(std::unique_ptr<Expression> return_value);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kReturnStatement;
  }
  [[nodiscard]] const std::unique_ptr<Expression>& return_value() const {
    return return_value_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator==(const Node&& other) const override;
  bool operator!=(const Node& other) const override;
  bool operator!=(const Node&& other) const override;

 private:
  std::unique_ptr<Expression> return_value_;
};

class ExpressionStatement : public Statement {
 public:
  explicit ExpressionStatement(std::unique_ptr<Expression> expression);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kExpressionStatement;
  }
  [[nodiscard]] const std::unique_ptr<Expression>& expression() const {
    return expression_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator==(const Node&& other) const override;
  bool operator!=(const Node& other) const override;
  bool operator!=(const Node&& other) const override;

 private:
  std::unique_ptr<Expression> expression_;
};

class BlockStatement : public Statement {
 public:
  explicit BlockStatement(std::vector<std::unique_ptr<Statement>> statements);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kBlockStatement;
  }
  [[nodiscard]] const std::vector<std::unique_ptr<Statement>>& statements()
      const {
    return statements_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator==(const Node&& other) const override;
  bool operator!=(const Node& other) const override;
  bool operator!=(const Node&& other) const override;

 private:
  std::vector<std::unique_ptr<Statement>> statements_;
};

}  // namespace monkey::ast

#endif  // MONKEY_AST_STMT_H