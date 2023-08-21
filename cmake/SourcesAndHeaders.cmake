set(sources
    src/lexer/lexer.cpp
    src/lexer/token.cpp
    src/ast/ast.cpp
    src/ast/expr.cpp
    src/ast/stmt.cpp
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
)

set(test_sources
  src/lexer/lexer_test.cpp
)
