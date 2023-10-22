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
  LetStatement(std::shared_ptr<Identifier> name,
               std::shared_ptr<Expression> value);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kLetStatement;
  }
  [[nodiscard]] const std::shared_ptr<Identifier>& name() const {
    return name_;
  }
  [[nodiscard]] const std::shared_ptr<Expression>& value() const {
    return value_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::shared_ptr<Identifier> name_;
  std::shared_ptr<Expression> value_;
};

class ReturnStatement : public Statement {
 public:
  explicit ReturnStatement(std::shared_ptr<Expression> return_value);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kReturnStatement;
  }
  [[nodiscard]] const std::shared_ptr<Expression>& return_value() const {
    return return_value_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::shared_ptr<Expression> return_value_;
};

class ExpressionStatement : public Statement {
 public:
  explicit ExpressionStatement(std::shared_ptr<Expression> expression);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kExpressionStatement;
  }
  [[nodiscard]] const std::shared_ptr<Expression>& expression() const {
    return expression_;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Node& other) const override;
  bool operator!=(const Node& other) const override;

 private:
  std::shared_ptr<Expression> expression_;
};

class BlockStatement : public Statement {
 public:
  explicit BlockStatement(std::vector<std::shared_ptr<Statement>> statements);

  [[nodiscard]] NodeType type() const override {
    return NodeType::kBlockStatement;
  }
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

#endif  // MONKEY_AST_STMT_H