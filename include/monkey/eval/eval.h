#ifndef MONKEY_EVAL_EVAL_H_
#define MONKEY_EVAL_EVAL_H_

#include <monkey/ast/ast.h>
#include <monkey/object/object.h>

#include <memory>

namespace monkey::eval {

std::shared_ptr<object::Object> eval(const ast::Node& node,
                                     std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalProgram(const ast::Program& program,
                                            std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalLetStatement(
    const ast::LetStatement& let_statement, std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalReturnStatement(
    const ast::ReturnStatement& return_statement,
    std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalExpressionStatement(
    const ast::ExpressionStatement& expression_statement,
    std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalBlockStatement(
    const ast::BlockStatement& block_statement, std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalIdentifier(
    const ast::Identifier& identifier, std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalIntegerLiteral(
    const ast::IntegerLiteral& integer_literal, std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalBooleanLiteral(
    const ast::BooleanLiteral& boolean_literal, std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalStringLiteral(
    const ast::StringLiteral& string_literal, std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalArrayLiteral(
    const ast::ArrayLiteral& array_literal, std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalHashLiteral(
    const ast::HashLiteral& hash_literal, std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalPrefixExpression(
    const ast::PrefixExpression& prefix_expression,
    std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalInfixExpression(
    const ast::InfixExpression& infix_expression,
    std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalIfExpression(
    const ast::IfExpression& if_expression, std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalFunctionLiteral(
    const ast::FunctionLiteral& function_literal,
    std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalCallExpression(
    const ast::CallExpression& call_expression, std::shared_ptr<object::Env>&);

std::shared_ptr<object::Object> evalIndexExpression(
    const ast::IndexExpression& index_expression,
    std::shared_ptr<object::Env>&);

}  // namespace monkey::eval

#endif  // MONKEY_EVAL_EVAL_H_
