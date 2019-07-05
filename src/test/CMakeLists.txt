
project(vegetable-script-test)
add_definitions(-std=c++11)
include_directories(${PROJECT_SOURCE_DIR}/../core)
add_executable(vegetable-script-test
    main.cpp
    test_lexer.cpp
    test_file.cpp
    test_statement_analyzer.cpp
    test_double_invoker.cpp)

target_link_libraries(vegetable-script-test vegetable-script-core)


INSTALL(TARGETS vegetable-script-test
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/../../installed/bin
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/../../installed/lib
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/../../installed/lib)