#include <monkey/ast/ast.h>
#include <monkey/ast/expr.h>
#include <monkey/ast/stmt.h>
#include <monkey/eval/builtin.h>
#include <monkey/eval/eval.h>
#include <monkey/lexer/token.h>
#include <monkey/object/env.h>
#include <monkey/object/object.h>

#include <cstddef>
#include <memory>
#include <vector>

namespace monkey::eval {

std::shared_ptr<object::Object> eval(const ast::Node& node,
                                     std::shared_ptr<object::Env>& env) {
  switch (node.type()) {
    case ast::NodeType::kProgram:
      return evalProgram(dynamic_cast<const ast::Program&>(node), env);
    case ast::NodeType::kLetStatement:
      return evalLetStatement(dynamic_cast<const ast::LetStatement&>(node),
                              env);
    case ast::NodeType::kReturnStatement:
      return evalReturnStatement(
          dynamic_cast<const ast::ReturnStatement&>(node), env);
    case ast::NodeType::kExpressionStatement:
      return evalExpressionStatement(
          dynamic_cast<const ast::ExpressionStatement&>(node), env);
    case ast::NodeType::kBlockStatement:
      return evalBlockStatement(dynamic_cast<const ast::BlockStatement&>(node),
                                env);

    case ast::NodeType::kIdentifier:
      return evalIdentifier(dynamic_cast<const ast::Identifier&>(node), env);
    case ast::NodeType::kIntegerLiteral:
      return evalIntegerLiteral(dynamic_cast<const ast::IntegerLiteral&>(node),
                                env);
    case ast::NodeType::kBooleanLiteral:
      return evalBooleanLiteral(dynamic_cast<const ast::BooleanLiteral&>(node),
                                env);
    case ast::NodeType::kStringLiteral:
      return evalStringLiteral(dynamic_cast<const ast::StringLiteral&>(node),
                               env);
    case ast::NodeType::kArrayLiteral:
      return evalArrayLiteral(dynamic_cast<const ast::ArrayLiteral&>(node),
                              env);
    case ast::NodeType::kHashLiteral:
      return evalHashLiteral(dynamic_cast<const ast::HashLiteral&>(node), env);
    case ast::NodeType::kPrefixExpression:
      return evalPrefixExpression(
          dynamic_cast<const ast::PrefixExpression&>(node), env);
    case ast::NodeType::kInfixExpression:
      return evalInfixExpression(
          dynamic_cast<const ast::InfixExpression&>(node), env);
    case ast::NodeType::kIfExpression:
      return evalIfExpression(dynamic_cast<const ast::IfExpression&>(node),
                              env);
    case ast::NodeType::kFunctionLiteral:
      return evalFunctionLiteral(
          dynamic_cast<const ast::FunctionLiteral&>(node), env);
    case ast::NodeType::kCallExpression:
      return evalCallExpression(dynamic_cast<const ast::CallExpression&>(node),
                                env);
    case ast::NodeType::kIndexExpression:
      return evalIndexExpression(
          dynamic_cast<const ast::IndexExpression&>(node), env);
    default:
      return nullptr;
  }
}

std::shared_ptr<object::Object> evalProgram(const ast::Program& program,
                                            std::shared_ptr<object::Env>& env) {
  std::shared_ptr<object::Object> result;
  for (const auto& statement : program.statements()) {
    result = eval(*statement, env);

    if (result->type() == object::ObjectType::kReturnValue) {
      return dynamic_cast<object::ReturnValue&>(*result).value();
    }
    if (result->type() == object::ObjectType::kError) {
      return result;
    }
  }

  return result;
}

std::shared_ptr<object::Object> evalLetStatement(
    const ast::LetStatement& let_statement, std::shared_ptr<object::Env>& env) {
  auto value = eval(*let_statement.value(), env);
  if (value->type() == object::ObjectType::kError) {
    return value;
  }

  env->set(let_statement.name()->name(), value);
  return value;
}

std::shared_ptr<object::Object> evalReturnStatement(
    const ast::ReturnStatement& return_statement,
    std::shared_ptr<object::Env>& env) {
  auto value = eval(*return_statement.return_value(), env);
  if (value->type() == object::ObjectType::kError) {
    return value;
  }

  return std::make_shared<object::ReturnValue>(value);
}

std::shared_ptr<object::Object> evalExpressionStatement(
    const ast::ExpressionStatement& expression_statement,
    std::shared_ptr<object::Env>& env) {
  return eval(*expression_statement.expression(), env);
}

std::shared_ptr<object::Object> evalBlockStatement(
    const ast::BlockStatement& block_statement,
    std::shared_ptr<object::Env>& env) {
  auto subenv = std::make_shared<object::Env>(env);
  std::shared_ptr<object::Object> result;
  for (const auto& statement : block_statement.statements()) {
    result = eval(*statement, subenv);

    if (result->type() == object::ObjectType::kReturnValue ||
        result->type() == object::ObjectType::kError) {
      return result;
    }
  }

  return result;
}

std::shared_ptr<object::Object> evalIdentifier(
    const ast::Identifier& identifier, std::shared_ptr<object::Env>& env) {
  auto value = env->get(identifier.name());
  if (value) {
    return value;
  }

  return error::unknown_identifier(identifier.name());
}

std::shared_ptr<object::Object> evalIntegerLiteral(
    const ast::IntegerLiteral& integer_literal, std::shared_ptr<object::Env>&) {
  return std::make_shared<object::Integer>(integer_literal.value());
}

std::shared_ptr<object::Object> evalBooleanLiteral(
    const ast::BooleanLiteral& boolean_literal, std::shared_ptr<object::Env>&) {
  return std::make_shared<object::Boolean>(boolean_literal.value());
}

std::shared_ptr<object::Object> evalStringLiteral(
    const ast::StringLiteral& string_literal, std::shared_ptr<object::Env>&) {
  return std::make_shared<object::String>(string_literal.value());
}

std::shared_ptr<object::Object> evalArrayLiteral(
    const ast::ArrayLiteral& array_literal, std::shared_ptr<object::Env>& env) {
  std::vector<std::shared_ptr<object::Object>> elements;
  for (const auto& element : array_literal.elements()) {
    auto evaluated = eval(*element, env);
    if (evaluated->type() == object::ObjectType::kError) {
      return evaluated;
    }
    elements.push_back(evaluated);
  }

  return std::make_shared<object::Array>(elements);
}

std::shared_ptr<object::Object> evalHashLiteral(
    const ast::HashLiteral& hash_literal, std::shared_ptr<object::Env>& env) {
  object::Hash::HashType pairs;
  for (const auto& [key, value] : hash_literal.pairs()) {
    auto evaluated_key = eval(*key, env);
    if (evaluated_key->type() == object::ObjectType::kError) {
      return evaluated_key;
    }

    auto evaluated_value = eval(*value, env);
    if (evaluated_value->type() == object::ObjectType::kError) {
      return evaluated_value;
    }

    pairs.insert({evaluated_key, evaluated_value});
  }

  return std::make_shared<object::Hash>(pairs);
}

std::shared_ptr<object::Object> evalPrefixExpression(
    const ast::PrefixExpression& prefix_expression,
    std::shared_ptr<object::Env>& env) {
  auto right = eval(*prefix_expression.right(), env);
  if (right->type() == object::ObjectType::kError) {
    return right;
  }

  switch (prefix_expression.op()) {
    case lexer::TokenType::kBang:
      switch (right->type()) {
        case object::ObjectType::kBoolean:
          return std::make_shared<object::Boolean>(
              !dynamic_cast<object::Boolean&>(*right).value());
        case object::ObjectType::kNull:
          return std::make_shared<object::Boolean>(true);
        default:
          return std::make_shared<object::Boolean>(false);
      }
    case lexer::TokenType::kMinus:
      if (right->type() != object::ObjectType::kInteger) {
        return error::wrong_prefix_operand("-", right->type());
      }
      return std::make_shared<object::Integer>(
          -dynamic_cast<object::Integer&>(*right).value());
    default:
      return nullptr;
  }
}

std::shared_ptr<object::Object> evalInfixExpression(
    const ast::InfixExpression& infix_expression,
    std::shared_ptr<object::Env>& env) {
  auto left = eval(*infix_expression.left(), env);
  if (left->type() == object::ObjectType::kError) {
    return left;
  }

  auto right = eval(*infix_expression.right(), env);
  if (right->type() == object::ObjectType::kError) {
    return right;
  }

  switch (infix_expression.op()) {
    case lexer::TokenType::kPlus:
      if (left->type() == object::ObjectType::kInteger &&
          right->type() == object::ObjectType::kInteger) {
        return std::make_shared<object::Integer>(
            dynamic_cast<object::Integer&>(*left).value() +
            dynamic_cast<object::Integer&>(*right).value());
      }
      if (left->type() == object::ObjectType::kString &&
          right->type() == object::ObjectType::kString) {
        return std::make_shared<object::String>(
            dynamic_cast<object::String&>(*left).value() +
            dynamic_cast<object::String&>(*right).value());
      }
      return error::wrong_infix_operands("+", left->type(), right->type());
    case lexer::TokenType::kMinus:
      if (left->type() != object::ObjectType::kInteger ||
          right->type() != object::ObjectType::kInteger) {
        return error::wrong_infix_operands("-", left->type(), right->type());
      }
      return std::make_shared<object::Integer>(
          dynamic_cast<object::Integer&>(*left).value() -
          dynamic_cast<object::Integer&>(*right).value());
    case lexer::TokenType::kAsterisk:
      if (left->type() != object::ObjectType::kInteger ||
          right->type() != object::ObjectType::kInteger) {
        return error::wrong_infix_operands("*", left->type(), right->type());
      }
      return std::make_shared<object::Integer>(
          dynamic_cast<object::Integer&>(*left).value() *
          dynamic_cast<object::Integer&>(*right).value());
    case lexer::TokenType::kSlash:
      if (left->type() != object::ObjectType::kInteger ||
          right->type() != object::ObjectType::kInteger) {
        return error::wrong_infix_operands("/", left->type(), right->type());
      }
      if (dynamic_cast<object::Integer&>(*right).value() == 0) {
        return error::division_by_zero();
      }
      return std::make_shared<object::Integer>(
          dynamic_cast<object::Integer&>(*left).value() /
          dynamic_cast<object::Integer&>(*right).value());
    case lexer::TokenType::kLessThan:
      if (left->type() != object::ObjectType::kInteger ||
          right->type() != object::ObjectType::kInteger) {
        return error::wrong_infix_operands("<", left->type(), right->type());
      }
      return std::make_shared<object::Boolean>(
          dynamic_cast<object::Integer&>(*left).value() <
          dynamic_cast<object::Integer&>(*right).value());
    case lexer::TokenType::kGreaterThan:
      if (left->type() != object::ObjectType::kInteger ||
          right->type() != object::ObjectType::kInteger) {
        return error::wrong_infix_operands(">", left->type(), right->type());
      }
      return std::make_shared<object::Boolean>(
          dynamic_cast<object::Integer&>(*left).value() >
          dynamic_cast<object::Integer&>(*right).value());
    case lexer::TokenType::kEqual:
      return std::make_shared<object::Boolean>(*left == *right);
    case lexer::TokenType::kNotEqual:
      return std::make_shared<object::Boolean>(*left != *right);
    default:
      return nullptr;
  }
}

std::shared_ptr<object::Object> evalIfExpression(
    const ast::IfExpression& if_expression, std::shared_ptr<object::Env>& env) {
  auto condition = eval(*if_expression.condition(), env);
  if (condition->type() == object::ObjectType::kError) {
    return condition;
  }

  if (*condition != *std::make_shared<object::Boolean>(false) &&
      *condition != *std::make_shared<object::Null>()) {
    return eval(*if_expression.consequence(), env);
  }

  if (if_expression.alternative()) {
    return eval(*if_expression.alternative(), env);
  }

  return std::make_shared<object::Null>();
}

std::shared_ptr<object::Object> evalFunctionLiteral(
    const ast::FunctionLiteral& function_literal,
    std::shared_ptr<object::Env>& env) {
  return std::make_shared<object::Function>(function_literal.parameters(),
                                            function_literal.body(), env);
}

std::shared_ptr<object::Object> evalCallExpression(
    const ast::CallExpression& call_expression,
    std::shared_ptr<object::Env>& env) {
  auto function = eval(*call_expression.function(), env);
  if (function->type() == object::ObjectType::kError) {
    return function;
  }

  std::vector<std::shared_ptr<object::Object>> args;
  for (const auto& arg : call_expression.arguments()) {
    auto evaluated = eval(*arg, env);
    if (evaluated->type() == object::ObjectType::kError) {
      return evaluated;
    }
    args.push_back(evaluated);
  }

  switch (function->type()) {
    case object::ObjectType::kFunction: {
      auto function_object = dynamic_cast<object::Function&>(*function);
      if (args.size() != function_object.parameters().size()) {
        return error::wrong_number_of_arguments(
            function_object.to_string(), function_object.parameters().size(),
            args.size());
      }
      auto subenv = std::make_shared<object::Env>(function_object.env());
      for (size_t i = 0; i < args.size(); ++i) {
        subenv->set(function_object.parameters()[i]->name(), args[i]);
      }

      auto evaluated = eval(*function_object.body(), subenv);
      if (evaluated->type() == object::ObjectType::kReturnValue) {
        return dynamic_cast<object::ReturnValue&>(*evaluated).value();
      }
      return evaluated;
    }
    case object::ObjectType::kBuiltin: {
      auto builtin = dynamic_cast<object::Builtin&>(*function);
      return builtin.function()(args);
    }
    default:
      return error::wrong_argument_type("call", object::ObjectType::kFunction,
                                        function->type());
  }
}

std::shared_ptr<object::Object> evalIndexExpression(
    const ast::IndexExpression& index_expression,
    std::shared_ptr<object::Env>& env) {
  auto left = eval(*index_expression.left(), env);
  if (left->type() == object::ObjectType::kError) {
    return left;
  }

  auto index = eval(*index_expression.index(), env);
  if (index->type() == object::ObjectType::kError) {
    return index;
  }

  switch (left->type()) {
    case object::ObjectType::kArray: {
      auto& array = dynamic_cast<object::Array&>(*left);
      if (index->type() != object::ObjectType::kInteger) {
        return error::wrong_index_operands(left->type(), index->type());
      }
      auto i = dynamic_cast<object::Integer&>(*index).value();
      if (i < 0 || static_cast<size_t>(i) >= array.elements().size()) {
        return std::make_shared<object::Null>();
      }
      return array.elements()[static_cast<size_t>(i)];
    }
    case object::ObjectType::kHash: {
      auto& hash = dynamic_cast<object::Hash&>(*left);
      if (index->type() != object::ObjectType::kBoolean &&
          index->type() != object::ObjectType::kInteger &&
          index->type() != object::ObjectType::kString) {
        return error::wrong_index_operands(left->type(), index->type());
      }
      auto it = hash.pairs().find(index);
      if (it == hash.pairs().end()) {
        return std::make_shared<object::Null>();
      }
      return it->second;
    }
    default:
      return error::wrong_index_operands(left->type(), index->type());
  }
}

}  // namespace monkey::eval
