project(net-project)
cmake_minimum_required(VERSION 3.24)

# message(>>>>>>>>>>${CMAKE_PROJECT_NAME}>>>>>>>>>>>)

# print
# message(${CMAKE_SOURCE_DIR})
# message(${CMAKE_BINARY_DIR})
# message(${CMAKE_CURRENT_BINARY_DIR})

message(>>>>>>>>>>PROJ>>>>>>>>>>>)

if(EMSCRIPTEN)
    message(STATUS "Using emscripten!")
else()
    message(STATUS "Not using emscripten!")
endif()

# lib
add_subdirectory(lib)
add_subdirectory(test)
# add_subdirectory(jni)

# wasm
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/wasm)

# add_custom_target(wasm_t1 ALL
#     COMMAND emcmake cmake ${CMAKE_CURRENT_SOURCE_DIR}/wasm
#     COMMAND make
#     DEPENDS ${CMAKE_BINARY_DIR}/wasm
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/wasm
#     COMMENT "wasm - emcmake"
#   VERBATIM
# )

# message(<<<<<<<<<<PROJ<<<<<<<<<<<)

# test
include(CTest)
enable_testing()