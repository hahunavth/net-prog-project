project (net-wasm-wrapper)
cmake_minimum_required(VERSION 3.24)

option(JS_ONLY "Compiles to native JS (No WASM)" OFF)

# add_definitions(-std=c++11 -O3)
include_directories(include)

file(GLOB WASM_SRC
     "./*.c"
     "./*.h"
)

# list(APPEND CMAKE_MODULE_PATH 
# "/home/kryo/emsdk/upstream/emscripten/emcmake"
# /home/kryo/emsdk/upstream/emscripten/cmake/Modules
# # "/home/kryo/emsdk/upstream/emscripten/cmake/Modules"
# # "/home/kryo/emsdk/upstream/emscripten/cmake/Modules"
# # "/home/kryo/emsdk/upstream/emscripten/cmake/Modules"
# )
# message(${CMAKE_MODULE_PATH})

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# add_compile_options("-s" "USE_FREETYPE=1")

message(>>>>>>>>>>WASM>>>>>>>>>>>)

if(EMSCRIPTEN)
    message(STATUS "Using emscripten!")
else()
    message(STATUS "Not using emscripten!")
endif()

set(CMAKE_TOOLCHAIN_FILE "/home/kryo/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
include_directories("/home/kryo/emsdk/upstream/emscripten/cache/sysroot/include/")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib ${CMAKE_CURRENT_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/public")

add_executable(a ${WASM_SRC} )

if(JS_ONLY)
	message(STATUS "Setting compilation target to native JavaScript")
	set(CMAKE_EXECUTABLE_SUFFIX ".js")
	set_target_properties(a PROPERTIES LINK_FLAGS "-s WASM=0 -s EXPORTED_FUNCTIONS='[_main]'")
else(JS_ONLY)
	message(STATUS "Setting compilation target to WASM")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
	set_target_properties(a PROPERTIES LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS='[_main]'  -s NO_EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS=['ccall, cwrap'] ")
endif(JS_ONLY)

message(<<<<<<<<<<WASM<<<<<<<<<<<)
