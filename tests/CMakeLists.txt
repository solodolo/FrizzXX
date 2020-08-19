cmake_minimum_required(VERSION 3.16)

set(TNAME Frizz++_tests)

project(${TNAME})

set(SOURCES
	lexer_tests.cpp
	parser_tests.cpp
	abstract_syntax_tree_tests.cpp
	config_tests.cpp
)

find_package(GTest REQUIRED)

include_directories(${GTEST_BOTH_LIBRARIES} ${CMAKE_SOURCE_DIR}/src)

add_executable(${TNAME} ${SOURCES})

target_link_libraries(${TNAME} PUBLIC ${GTEST_BOTH_LIBRARIES} ${LIBNAME} pthread)

add_test(
	NAME ${TNAME}
	COMMAND ${TNAME}
)