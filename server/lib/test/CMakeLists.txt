project (net-pj-lib-test)
cmake_minimum_required(VERSION 3.24)

file(GLOB TEST_SRC
"*.c"
# "*.h"
"*?/*.c"
# "*?/*.h"
)

message(">>>>>>>>>>TEST>>>>>>>>>>>")


# ignore
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  message("Add exec: ")
endif()
foreach(exec_file IN LISTS TEST_SRC)
  string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" output_file "${exec_file}")
  string(REPLACE ".c" "" output_name "${output_file}")
  string(REPLACE "/" "-" output_name "${output_name}")
  if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    message(
      "  - ${output_name} ${output_file}"
    )
  endif()
  add_executable(${output_name} ${output_file} )
  target_link_libraries(${output_name} chatptcl)
endforeach()

message("<<<<<<<<<<TEST<<<<<<<<<<<")
