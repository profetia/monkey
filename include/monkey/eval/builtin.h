#ifndef MONKEY_EVAL_BUILTIN_H_
#define MONKEY_EVAL_BUILTIN_H_

#include <monkey/object/object.h>

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace monkey::eval {

namespace builtin {

std::shared_ptr<object::Object> len(
    const std::vector<std::shared_ptr<object::Object>>& args);

std::shared_ptr<object::Object> first(
    const std::vector<std::shared_ptr<object::Object>>& args);

std::shared_ptr<object::Object> last(
    const std::vector<std::shared_ptr<object::Object>>& args);

std::shared_ptr<object::Object> rest(
    const std::vector<std::shared_ptr<object::Object>>& args);

std::shared_ptr<object::Object> push(
    const std::vector<std::shared_ptr<object::Object>>& args);

std::shared_ptr<object::Object> puts(
    const std::vector<std::shared_ptr<object::Object>>& args);

}  // namespace builtin

namespace error {

std::shared_ptr<object::Object> unknown_identifier(const std::string& name);

std::shared_ptr<object::Object> wrong_number_of_arguments(
    const std::string& name, size_t expected, size_t got);

std::shared_ptr<object::Object> wrong_argument_type(const std::string& name,
                                                    object::ObjectType expected,
                                                    object::ObjectType got);

std::shared_ptr<object::Object> division_by_zero();

std::shared_ptr<object::Object> wrong_prefix_operand(
    const std::string& operator_, object::ObjectType type);

std::shared_ptr<object::Object> wrong_infix_operands(
    const std::string& operator_, object::ObjectType left,
    object::ObjectType right);

std::shared_ptr<object::Object> wrong_index_operands(object::ObjectType left,
                                                     object::ObjectType right);

std::shared_ptr<object::Object> wrong_call_operand(const std::string& name,
                                                   object::ObjectType type);

}  // namespace error

}  // namespace monkey::eval

#endif  // MONKEY_EVAL_BUILTIN_H_
