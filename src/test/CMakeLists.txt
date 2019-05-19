
project(vegetable-script-test)


add_executable(vegetable-script-test main.cpp)
target_link_libraries(vegetable-script-test vegetable-script-core)


INSTALL(TARGETS vegetable-script-test
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/../../installed/bin
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/../../installed/lib
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/../../installed/lib)