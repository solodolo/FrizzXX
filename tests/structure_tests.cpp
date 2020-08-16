/*
 * structure_tests.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: dmmettlach
 */
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

#include "structures.h"

static const std::string TEST_FILE_DIR = "./tests/test_files";

std::string get_expected(std::filesystem::path path) {
  std::ifstream input(path);
  std::string expected;

  char c;
  while(input.get(c)) {
    expected.push_back(c);
  }

  return expected;
}
TEST(AssignmentStructTests, ReadsFileContents) {
  Frizz::AssignmentStruct structure("src", "test1.md");
  std::string contents = structure.evaluate(TEST_FILE_DIR);

  std::filesystem::path p(TEST_FILE_DIR);
  p /= "test1.md";

  std::string expected = get_expected(p);
  
  ASSERT_EQ(expected, contents);
}