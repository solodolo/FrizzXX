cmake_minimum_required(VERSION 3.16)

set(TNAME Frizz++_tests)

project(${TNAME})

set(SOURCES
	lexer_tests.cpp
	parser_tests.cpp
	abstract_syntax_tree_tests.cpp
	config_tests.cpp
	runner_tests.cpp
	file_util_tests.cpp
)

#find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(${TNAME} ${SOURCES})

target_link_libraries(${TNAME} PUBLIC gtest gtest_main ${LIBNAME} pthread)

add_test(
	NAME ${TNAME}
	COMMAND ${TNAME}
)