#include <fmt/core.h>
#include <monkey/eval/builtin.h>
#include <monkey/object/object.h>

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace monkey::eval {

namespace builtin {

std::shared_ptr<object::Object> len(
    const std::vector<std::shared_ptr<object::Object>>& args) {
  if (args.size() != 1) {
    return error::wrong_number_of_arguments("len", 1, args.size());
  }

  switch (args[0]->type()) {
    case object::ObjectType::kString:
      return std::make_shared<object::Integer>(
          dynamic_cast<object::String&>(*args[0]).value().size());
    case object::ObjectType::kArray:
      return std::make_shared<object::Integer>(
          dynamic_cast<object::Array&>(*args[0]).elements().size());
    default:
      return error::wrong_argument_type("len", object::ObjectType::kString,
                                        args[0]->type());
  }
}

std::shared_ptr<object::Object> first(
    const std::vector<std::shared_ptr<object::Object>>& args) {
  if (args.size() != 1) {
    return error::wrong_number_of_arguments("first", 1, args.size());
  }

  if (args[0]->type() != object::ObjectType::kArray) {
    return error::wrong_argument_type("first", object::ObjectType::kArray,
                                      args[0]->type());
  }

  auto& array = dynamic_cast<object::Array&>(*args[0]);
  if (array.elements().empty()) {
    return std::make_unique<object::Null>();
  }

  return array.elements()[0];
}

std::shared_ptr<object::Object> last(
    const std::vector<std::shared_ptr<object::Object>>& args) {
  if (args.size() != 1) {
    return error::wrong_number_of_arguments("last", 1, args.size());
  }

  if (args[0]->type() != object::ObjectType::kArray) {
    return error::wrong_argument_type("last", object::ObjectType::kArray,
                                      args[0]->type());
  }

  auto& array = dynamic_cast<object::Array&>(*args[0]);
  if (array.elements().empty()) {
    return std::make_unique<object::Null>();
  }

  return array.elements()[array.elements().size() - 1];
}

std::shared_ptr<object::Object> rest(
    const std::vector<std::shared_ptr<object::Object>>& args) {
  if (args.size() != 1) {
    return error::wrong_number_of_arguments("rest", 1, args.size());
  }

  if (args[0]->type() != object::ObjectType::kArray) {
    return error::wrong_argument_type("rest", object::ObjectType::kArray,
                                      args[0]->type());
  }

  auto& array = dynamic_cast<object::Array&>(*args[0]);
  if (array.elements().empty()) {
    return std::make_unique<object::Null>();
  }

  std::vector<std::shared_ptr<object::Object>> elements;
  for (size_t i = 1; i < array.elements().size(); ++i) {
    elements.push_back(array.elements()[i]);
  }

  return std::make_shared<object::Array>(elements);
}

std::shared_ptr<object::Object> push(
    const std::vector<std::shared_ptr<object::Object>>& args) {
  if (args.size() != 2) {
    return error::wrong_number_of_arguments("push", 2, args.size());
  }

  if (args[0]->type() != object::ObjectType::kArray) {
    return error::wrong_argument_type("push", object::ObjectType::kArray,
                                      args[0]->type());
  }

  auto& array = dynamic_cast<object::Array&>(*args[0]);
  std::vector<std::shared_ptr<object::Object>> elements;
  for (const auto& element : array.elements()) {
    elements.push_back(element);
  }
  elements.push_back(args[1]);

  return std::make_shared<object::Array>(elements);
}

std::shared_ptr<object::Object> puts(
    const std::vector<std::shared_ptr<object::Object>>& args) {
  for (const auto& arg : args) {
    fmt::print("{}\n", arg->to_string());
  }

  return std::make_unique<object::Null>();
}

}  // namespace builtin

namespace error {

std::shared_ptr<object::Object> unknown_identifier(const std::string& name) {
  return std::make_shared<object::Error>(
      fmt::format("identifier not found: {}", name));
}

std::shared_ptr<object::Object> wrong_number_of_arguments(
    const std::string& name, size_t expected, size_t got) {
  return std::make_shared<object::Error>(
      fmt::format("wrong number of arguments for {}: expected {}, got {}", name,
                  expected, got));
}

std::shared_ptr<object::Object> wrong_argument_type(const std::string& name,
                                                    object::ObjectType expected,
                                                    object::ObjectType got) {
  return std::make_shared<object::Error>(
      fmt::format("wrong argument type for {}: expected {}, got {}", name,
                  object::to_string(expected), object::to_string(got)));
}

std::shared_ptr<object::Object> division_by_zero() {
  return std::make_shared<object::Error>("division by zero");
}

std::shared_ptr<object::Object> wrong_prefix_operand(
    const std::string& operator_, object::ObjectType type) {
  return std::make_shared<object::Error>(
      fmt::format("wrong operand type for {}: {}{}", operator_, operator_,
                  object::to_string(type)));
}

std::shared_ptr<object::Object> wrong_infix_operands(
    const std::string& operator_, object::ObjectType left,
    object::ObjectType right) {
  return std::make_shared<object::Error>(fmt::format(
      "wrong operand types for {}: {} {} {}", operator_,
      object::to_string(left), operator_, object::to_string(right)));
}

std::shared_ptr<object::Object> wrong_index_operands(object::ObjectType left,
                                                     object::ObjectType right) {
  return std::make_shared<object::Error>(
      fmt::format("wrong index types for []: {}[{}]", object::to_string(left),
                  object::to_string(right)));
}

std::shared_ptr<object::Object> wrong_call_operand(const std::string& name,
                                                   object::ObjectType type) {
  return std::make_shared<object::Error>(fmt::format(
      "wrong operand type for (): {}()", name, object::to_string(type)));
}

}  // namespace error

}  // namespace monkey::eval