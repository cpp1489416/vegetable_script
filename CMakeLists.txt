
add_definitions(-std=c++11)
include_directories(${PROJECT_BINARY_DIR}/dist/include)
link_directories(${PROJECT_BINARY_DIR}/dist/lib)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)


ADD_SUBDIRECTORY(src/core)
ADD_SUBDIRECTORY(src/test)

INSTALL(
    DIRECTORY  ${PROJECT_BINARY_DIR}/dist/include
    DESTINATION ${PROJECT_BINARY_DIR}/installed
    FILES_MATCHING PATTERN *.h)
