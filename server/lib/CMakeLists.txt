project(net-pj-lib 
# VERSION 1.0.0
LANGUAGES C
)
cmake_minimum_required(VERSION 3.24)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif (NOT CMAKE_BUILD_TYPE)

# Param
#   TODO: Add exec.c file here!
list(APPEND EXEC_C_FILES 
  # file has main function!
  "main.c"
)

# Add debug flag
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")

# include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)

# set(CMAKE_TOOLCHAIN_FILE "/home/kryo/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
# include_directories("/home/kryo/emsdk/upstream/emscripten/cache/sysroot/include/")


# include all file
file(GLOB PJ_SRC
     "*.c"
     "*.h"
     "common/*.h"
     "common/*.c"
     "utils/*.h"
     "utils/*.c"
     "msg/*.h"
     "msg/*.c"
     "msg/*/*.h"
     "msg/*/*.c"
    #  "cgen/*.h"
    #  "cgen/*.c"
)

message(">>>>>>>>>>LIB_>>>>>>>>>>>")

add_subdirectory(cgen)
# add_subdirectory(test)
# add_subdirectory(wasm)

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  if(EMSCRIPTEN)
      message("Using emscripten!")
  else()
      message("Not using emscripten!")
  endif()
endif()

# Filter exec file in lib_list
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message("Filter file: ")
endif()
foreach(exec_file IN LISTS EXEC_C_FILES)
  # message("${CMAKE_CURRENT_SOURCE_DIR}/${exec_file}")
  if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    message("  - ${exec_file}")
  endif()
  list(REMOVE_ITEM PJ_SRC "${CMAKE_CURRENT_SOURCE_DIR}/${exec_file}")
endforeach()
# print lib path
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message("Library path:")
  foreach(f IN LISTS PJ_SRC)
    message(${f})
  endforeach()
endif()

add_library(chatptcl STATIC ${PJ_SRC})

# add exec
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
message("Add exec: ")
endif()
foreach(exec_file IN LISTS EXEC_C_FILES)
  string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" output_file "${exec_file}")
  string(REPLACE ".c" "" output_name "${output_file}")
  if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    message(
      "  - ${output_name} ${output_file}"
    )
  endif()
  add_executable(${output_name} ${output_file} )
  target_link_libraries(${output_name} chatptcl)
endforeach()

message("<<<<<<<<<<LIB_<<<<<<<<<<<")


# test
include(CTest)

add_test(NAME UtilsFile COMMAND ./test/utils-test-file)
add_test(NAME UtilsContentStr COMMAND ./test/utils-test-content-str)
add_test(NAME UtilsMsg COMMAND ./test/utils-test-msg)
# add_test(NAME NetTestServer COMMAND ./test/net-test-server & exit; )

#[[
# Ignored:
# Transform
# list(TRANSFORM PJ_SRC REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "")
# message(${PJ_SRC})
]]