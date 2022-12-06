cmake_minimum_required(VERSION 3.24)

file( GLOB_RECURSE CGEN_SRC
./bkcgen.c ./bkcgen.h
)

add_library(cgen ${CGEN_SRC})

set_source_files_properties(
  ${CGEN_SRC}
  PROPERTIES
  COMPILE_FLAGS "-w"
)