set(sources
    src/lexer/lexer.cpp
    src/lexer/token.cpp
    src/ast/ast.cpp
    src/ast/expr.cpp
    src/ast/stmt.cpp
    src/parser/error.cpp
    src/parser/expr.cpp
    src/parser/parser.cpp
    src/parser/reader.cpp
    src/parser/stmt.cpp
)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
    include/monkey/lexer/lexer.h
    include/monkey/lexer/token.h
    include/monkey/ast/ast.h
    include/monkey/ast/expr.h
    include/monkey/ast/stmt.h
    include/monkey/parser/error.h
    include/monkey/parser/expr.h
    include/monkey/parser/parser.h
    include/monkey/parser/reader.h
    include/monkey/parser/stmt.h
)

set(test_sources
  src/lexer/lexer_test.cpp
  src/parser/parser_test.cpp
)
