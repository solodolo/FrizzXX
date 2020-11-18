cmake_minimum_required(VERSION 3.16)

set(TNAME Frizz++_tests)

project(${TNAME})

set(SOURCES
	lexer_tests.cpp
	lexer_file_tests.cpp
	parser_tests.cpp
	abstract_syntax_tree_tests.cpp
	config_tests.cpp
	processor_tests.cpp
	file_util_tests.cpp
	test_helpers.cpp
	syntax_tree_visitor_tests.cpp
)

set(HEADERS
	test_helpers.h
)

#find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(${TNAME} ${SOURCES} ${HEADERS})

target_link_libraries(${TNAME} PUBLIC gtest gtest_main ${LIBNAME} pthread)

add_test(
	NAME ${TNAME}
	COMMAND ${TNAME}
)