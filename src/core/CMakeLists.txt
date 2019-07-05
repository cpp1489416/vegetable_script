
project(vegetable-script)
add_definitions(-std=c++11)

set(src_list
    common/string_util.cpp
    common/file_util.cpp
    parsing/token.cpp
    parsing/parsing_exception.cpp
    parsing/source_provider.cpp
    parsing/lexer.cpp
    parsing/ll1_expression_analyzer.cpp
    parsing/ll1_statement_analyzer.cpp
    visitor/ast_stringer.cpp)

add_library(vegetable-script-core STATIC ${src_list})
add_library(vegetable-script-core-shared SHARED ${src_list})

file(COPY .
    DESTINATION ${PROJECT_BINARY_DIR}/../../dist/include/vegetable_script
    FILES_MATCHING REGEX ".(h|inc)$")

INSTALL(TARGETS vegetable-script-core
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/../../installed/bin
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/../../installed/lib
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/../../installed/lib)