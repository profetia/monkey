set(sources
    src/lexer/lexer.cpp
    src/lexer/token.cpp
)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
    include/monkey/lexer/lexer.h
    include/monkey/lexer/token.h
)

set(test_sources
  src/lexer/lexer_test.cpp
)
