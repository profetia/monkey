#include <monkey/ast/ast.h>
#include <monkey/ast/expr.h>
#include <monkey/ast/stmt.h>
#include <monkey/object/object.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace monkey::object {

std::string to_string(ObjectType type) {
  switch (type) {
    case ObjectType::kInteger:
      return "INTEGER";
    case ObjectType::kBoolean:
      return "BOOLEAN";
    case ObjectType::kNull:
      return "NULL";
    case ObjectType::kReturnValue:
      return "RETURN";
    case ObjectType::kFunction:
      return "FUNCTION";
    case ObjectType::kString:
      return "STRING";
    case ObjectType::kArray:
      return "ARRAY";
    case ObjectType::kHash:
      return "HASH";
    case ObjectType::kBuiltin:
      return "BUILTIN";
    case ObjectType::kError:
      return "ERROR";
    case ObjectType::kQuote:
      return "QUOTE";
    case ObjectType::kMacro:
      return "MACRO";
    default:
      return "UNKNOWN";
  }
}

std::string to_string(const Object& obj) { return obj.to_string(); }

std::string to_string(const Object&& obj) { return obj.to_string(); }

Integer::Integer(int64_t value) : value_(value) {}

std::string Integer::to_string() const { return std::to_string(value_); }

bool Integer::operator==(const Object& other) const {
  if (other.type() != ObjectType::kInteger) {
    return false;
  }
  return value_ == dynamic_cast<const Integer&>(other).value_;
}

bool Integer::operator!=(const Object& other) const {
  return !(*this == other);
}

Boolean::Boolean(bool value) : value_(value) {}

std::string Boolean::to_string() const { return value_ ? "true" : "false"; }

bool Boolean::operator==(const Object& other) const {
  if (other.type() != ObjectType::kBoolean) {
    return false;
  }
  return value_ == dynamic_cast<const Boolean&>(other).value_;
}

bool Boolean::operator!=(const Object& other) const {
  return !(*this == other);
}

Null::Null() noexcept = default;

std::string Null::to_string() const { return "null"; }

bool Null::operator==(const Object& other) const {
  return other.type() == ObjectType::kNull;
}

bool Null::operator!=(const Object& other) const { return !(*this == other); }

ReturnValue::ReturnValue(std::shared_ptr<Object> value)
    : value_(std::move(value)) {}

std::string ReturnValue::to_string() const { return value_->to_string(); }

bool ReturnValue::operator==(const Object& other) const {
  if (other.type() != ObjectType::kReturnValue) {
    return false;
  }
  return *value_ == *dynamic_cast<const ReturnValue&>(other).value_;
}

bool ReturnValue::operator!=(const Object& other) const {
  return !(*this == other);
}

Function::Function(std::vector<std::unique_ptr<ast::Identifier>> parameters,
                   std::unique_ptr<ast::BlockStatement> body,
                   std::shared_ptr<Env> env)
    : parameters_(std::move(parameters)),
      body_(std::move(body)),
      env_(std::move(env)) {}

std::string Function::to_string() const {
  std::string out = "fn(";
  for (const auto& param : parameters_) {
    out += param->to_string() + ", ";
  }
  out += ") {\n" + body_->to_string() + "\n}";
  return out;
}

bool Function::operator==(const Object& other) const {
  if (other.type() != ObjectType::kFunction) {
    return false;
  }
  const auto& other_func = dynamic_cast<const Function&>(other);
  if (parameters_.size() != other_func.parameters_.size()) {
    return false;
  }
  for (size_t i = 0; i < parameters_.size(); ++i) {
    if (parameters_[i] != other_func.parameters_[i]) {
      return false;
    }
  }
  return body_->operator==(*other_func.body_);
}

bool Function::operator!=(const Object& other) const {
  return !(*this == other);
}

String::String(std::string value) : value_(std::move(value)) {}

std::string String::to_string() const { return value_; }

bool String::operator==(const Object& other) const {
  if (other.type() != ObjectType::kString) {
    return false;
  }
  return value_ == dynamic_cast<const String&>(other).value_;
}

bool String::operator!=(const Object& other) const { return !(*this == other); }

Array::Array(std::vector<std::shared_ptr<Object>> elements)
    : elements_(std::move(elements)) {}

std::string Array::to_string() const {
  std::string out = "[";
  for (const auto& element : elements_) {
    out += element->to_string() + ", ";
  }
  out += "]";
  return out;
}

bool Array::operator==(const Object& other) const {
  if (other.type() != ObjectType::kArray) {
    return false;
  }
  const auto& other_array = dynamic_cast<const Array&>(other);
  if (elements_.size() != other_array.elements_.size()) {
    return false;
  }
  for (size_t i = 0; i < elements_.size(); ++i) {
    if (*elements_[i] != *other_array.elements_[i]) {
      return false;
    }
  }
  return true;
}

bool Array::operator!=(const Object& other) const { return !(*this == other); }

Hash::Hash(Hash::HashType pairs) : pairs_(std::move(pairs)) {}

std::string Hash::to_string() const {
  std::string out = "{";
  for (const auto& [key, value] : pairs_) {
    out += key->to_string() + ": " + value->to_string() + ", ";
  }
  out += "}";
  return out;
}

bool Hash::operator==(const Object& other) const {
  if (other.type() != ObjectType::kHash) {
    return false;
  }
  const auto& other_hash = dynamic_cast<const Hash&>(other);
  if (pairs_.size() != other_hash.pairs_.size()) {
    return false;
  }

  return std::ranges::all_of(pairs_, [&other_hash](const auto& pair) {
    const auto it = other_hash.pairs_.find(pair.first);
    return it != other_hash.pairs_.end() && *pair.second == *it->second;
  });
}

bool Hash::operator!=(const Object& other) const { return !(*this == other); }

Builtin::Builtin(Builtin::FunctionType* fn) : fn_(std::move(fn)) {}

std::string Builtin::to_string() const { return "builtin function"; }

bool Builtin::operator==(const Object& other) const {
  if (other.type() != ObjectType::kBuiltin) {
    return false;
  }
  return fn_ == dynamic_cast<const Builtin&>(other).fn_;
}

bool Builtin::operator!=(const Object& other) const {
  return !(*this == other);
}

Error::Error(std::string message) : message_(std::move(message)) {}

std::string Error::to_string() const { return "ERROR: " + message_; }

bool Error::operator==(const Object& other) const {
  if (other.type() != ObjectType::kError) {
    return false;
  }
  return message_ == dynamic_cast<const Error&>(other).message_;
}

bool Error::operator!=(const Object& other) const { return !(*this == other); }

Quote::Quote(std::unique_ptr<ast::Node> node) : node_(std::move(node)) {}

std::string Quote::to_string() const {
  return "QUOTE(" + node_->to_string() + ")";
}

bool Quote::operator==(const Object& other) const {
  if (other.type() != ObjectType::kQuote) {
    return false;
  }
  return *node_ == *dynamic_cast<const Quote&>(other).node_;
}

bool Quote::operator!=(const Object& other) const { return !(*this == other); }

Macro::Macro(std::vector<std::unique_ptr<ast::Identifier>> parameters,
             std::unique_ptr<ast::BlockStatement> body,
             std::shared_ptr<Env> env)
    : parameters_(std::move(parameters)),
      body_(std::move(body)),
      env_(std::move(env)) {}

std::string Macro::to_string() const {
  std::string out = "macro(";
  for (const auto& param : parameters_) {
    out += param->to_string() + ", ";
  }
  out += ") {\n" + body_->to_string() + "\n}";
  return out;
}

bool Macro::operator==(const Object& other) const {
  if (other.type() != ObjectType::kMacro) {
    return false;
  }
  const auto& other_macro = dynamic_cast<const Macro&>(other);
  if (parameters_.size() != other_macro.parameters_.size()) {
    return false;
  }
  for (size_t i = 0; i < parameters_.size(); ++i) {
    if (parameters_[i] != other_macro.parameters_[i]) {
      return false;
    }
  }
  return body_->operator==(*other_macro.body_);
}

bool Macro::operator!=(const Object& other) const { return !(*this == other); }

}  // namespace monkey::object