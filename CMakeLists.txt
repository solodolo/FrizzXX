cmake_minimum_required(VERSION 3.16)

set(PNAME FrizzXX)
set(LIBNAME FrizzXX_lib)

project(${PNAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(googletest)

set(HEADERS
  src/lexer.h
  src/parser.h
  src/abstract_syntax_trees.h
  src/md_preprocessor.h
  src/frizz_config.h
  src/file_utility.h
  src/runner.h
  src/context_parser.h
)

set(SOURCES
  src/frizz++.cpp
  src/lexer.cpp
  src/parser.cpp
  src/abstract_syntax_trees.cpp
  src/md_preprocessor.cpp
  src/frizz_config.cpp
  src/file_utility.cpp
  src/runner.cpp
  src/context_parser.cpp
  src/syntax_tree_visitors.cpp
)

include_directories(${CMAKE_CURRENT_DIRECTORY}/src)

add_executable(${PNAME} ${SOURCES} ${HEADERS}) 
add_library(${LIBNAME} STATIC ${SOURCES} ${HEADERS})


target_include_directories(${PNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_include_directories(${LIBNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(tests)