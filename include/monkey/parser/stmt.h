#ifndef MONKEY_PARSER_STMT_H_
#define MONKEY_PARSER_STMT_H_

#include <monkey/ast/stmt.h>
#include <monkey/lexer/token.h>
#include <monkey/parser/reader.h>

namespace monkey::parser {

std::shared_ptr<ast::Statement> parse_statement(Reader& reader);

std::shared_ptr<ast::LetStatement> parse_let_statement(Reader& reader);

std::shared_ptr<ast::ReturnStatement> parse_return_statement(Reader& reader);

std::shared_ptr<ast::ExpressionStatement> parse_expression_statement(Reader& reader);

std::shared_ptr<ast::BlockStatement> parse_block_statement(Reader& reader);

}  // namespace monkey::parser

#endif  // MONKEY_PARSER_STMT_H_