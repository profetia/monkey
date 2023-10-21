#ifndef MONKEY_OBJECT_OBJECT_H_
#define MONKEY_OBJECT_OBJECT_H_

#include <monkey/ast/ast.h>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace monkey::object {

class Env;

enum class ObjectType {
  kInteger,
  kBoolean,
  kNull,
  kReturnValue,
  kFunction,
  kString,
  kArray,
  kHash,
  kBuiltin,
  kError,
  kQuote,
  kMacro,
};

std::string to_string(ObjectType type);

class Object {
 public:
  Object() = default;
  Object(const Object&) = default;
  Object(Object&&) = default;
  Object& operator=(const Object&) = default;
  Object& operator=(Object&&) = default;
  virtual ~Object() = default;

  [[nodiscard]] virtual ObjectType type() const = 0;

  [[nodiscard]] virtual std::string to_string() const = 0;

  virtual bool operator==(const Object& other) const = 0;
  virtual bool operator!=(const Object& other) const = 0;

  friend std::string to_string(const Object& obj);
};

class Integer : public Object {
 public:
  explicit Integer(int64_t value);

  [[nodiscard]] ObjectType type() const override {
    return ObjectType::kInteger;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] int64_t value() const { return value_; }

 private:
  int64_t value_;
};

class Boolean : public Object {
 public:
  explicit Boolean(bool value);

  [[nodiscard]] ObjectType type() const override {
    return ObjectType::kBoolean;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] bool value() const { return value_; }

 private:
  bool value_;
};

class Null : public Object {
 public:
  explicit Null() noexcept;

  [[nodiscard]] ObjectType type() const override { return ObjectType::kNull; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;
};

class ReturnValue : public Object {
 public:
  explicit ReturnValue(std::unique_ptr<Object> value);

  [[nodiscard]] ObjectType type() const override {
    return ObjectType::kReturnValue;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] const std::unique_ptr<Object>& value() const { return value_; }

 private:
  std::unique_ptr<Object> value_;
};

class Function : public Object {
 public:
  Function(std::vector<std::unique_ptr<ast::Identifier>> parameters,
           std::unique_ptr<ast::BlockStatement> body, std::shared_ptr<Env> env);

  [[nodiscard]] ObjectType type() const override {
    return ObjectType::kFunction;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] const std::vector<std::unique_ptr<ast::Identifier>>&
  parameters() const {
    return parameters_;
  }

  [[nodiscard]] const std::unique_ptr<ast::BlockStatement>& body() const {
    return body_;
  }

  [[nodiscard]] const std::shared_ptr<Env>& env() const { return env_; }

 private:
  std::vector<std::unique_ptr<ast::Identifier>> parameters_;
  std::unique_ptr<ast::BlockStatement> body_;
  std::shared_ptr<Env> env_;
};

class String : public Object {
 public:
  explicit String(std::string value);

  [[nodiscard]] ObjectType type() const override { return ObjectType::kString; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] const std::string& value() const { return value_; }

 private:
  std::string value_;
};

class Array : public Object {
 public:
  explicit Array(std::vector<std::unique_ptr<Object>> elements);

  [[nodiscard]] ObjectType type() const override { return ObjectType::kArray; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] const std::vector<std::unique_ptr<Object>>& elements() const {
    return elements_;
  }

 private:
  std::vector<std::unique_ptr<Object>> elements_;
};

class Hash : public Object {
 public:
  struct ObjectHasher {
    size_t operator()(const std::unique_ptr<Object>& obj) const {
      return std::hash<std::string>{}(obj->to_string());
    }
  };

  struct ObjectComparator {
    bool operator()(const std::unique_ptr<Object>& lhs,
                    const std::unique_ptr<Object>& rhs) const {
      return *lhs == *rhs;
    }
  };

  using HashType =
      std::unordered_map<std::unique_ptr<Object>, std::unique_ptr<Object>,
                         ObjectHasher, ObjectComparator>;

  explicit Hash(HashType pairs);

  [[nodiscard]] ObjectType type() const override { return ObjectType::kHash; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] const HashType& pairs() const { return pairs_; }

 private:
  HashType pairs_;
};

class Builtin : public Object {
 public:
  using FunctionType =
      std::unique_ptr<Object>(std::vector<std::unique_ptr<Object>>);

  explicit Builtin(FunctionType* fn);

  [[nodiscard]] ObjectType type() const override {
    return ObjectType::kBuiltin;
  }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] FunctionType* fn() const { return fn_; }

 private:
  FunctionType* fn_;
};

class Error : public Object {
 public:
  explicit Error(std::string message);

  [[nodiscard]] ObjectType type() const override { return ObjectType::kError; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] const std::string& message() const { return message_; }

 private:
  std::string message_;
};

class Quote : public Object {
 public:
  explicit Quote(std::unique_ptr<ast::Node> node);

  [[nodiscard]] ObjectType type() const override { return ObjectType::kQuote; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] const std::unique_ptr<ast::Node>& node() const { return node_; }

 private:
  std::unique_ptr<ast::Node> node_;
};

class Macro : public Object {
 public:
  Macro(std::vector<std::unique_ptr<ast::Identifier>> parameters,
        std::unique_ptr<ast::BlockStatement> body, std::shared_ptr<Env> env);

  [[nodiscard]] ObjectType type() const override { return ObjectType::kMacro; }

  [[nodiscard]] std::string to_string() const override;

  bool operator==(const Object& other) const override;
  bool operator!=(const Object& other) const override;

  [[nodiscard]] const std::vector<std::unique_ptr<ast::Identifier>>&
  parameters() const {
    return parameters_;
  }

  [[nodiscard]] const std::unique_ptr<ast::BlockStatement>& body() const {
    return body_;
  }

  [[nodiscard]] const std::shared_ptr<Env>& env() const { return env_; }

 private:
  std::vector<std::unique_ptr<ast::Identifier>> parameters_;
  std::unique_ptr<ast::BlockStatement> body_;
  std::shared_ptr<Env> env_;
};

}  // namespace monkey::object

#endif  // MONKEY_OBJECT_OBJECT_H_
