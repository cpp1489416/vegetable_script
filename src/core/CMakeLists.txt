
project(vegetable-script)

set(src_list mul.cpp)

add_library(vegetable-script-core SHARED ${src_list})
add_library(vegetable-script-core-static STATIC ${src_list})

file(COPY .
    DESTINATION ${PROJECT_BINARY_DIR}/../../dist/include
    FILES_MATCHING PATTERN *.h)

INSTALL(TARGETS vegetable-script-core
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/../../installed/bin
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/../../installed/lib
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/../../installed/lib)