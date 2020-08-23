/*
 * structure_tests.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: dmmettlach
 */
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

#include "abstract_syntax_trees.h"

static const std::string TEST_FILE_DIR = "./tests/test_files/sources";

std::string get_expected(std::filesystem::path path) {
  std::ifstream input(path);
  std::string expected;

  char c;
  while(input.get(c)) {
    expected.push_back(c);
  }

  return expected;
}

class AstTests : public ::testing::Test {
protected:
  Frizz::FrizzConfig config;
  Frizz::FileUtility util;
  Frizz::AstVisitor visitor;

  AstTests()
    : util(config)
    , visitor(util) {
    config.load_configuration("./tests/test_files/config/test2.json");
  }
};

TEST_F(AstTests, AssignmentAstReadsFileContents) {
  std::string expected = get_expected("./test_files/partials/test1.md");
  Frizz::AssignmentAst ast("src", "test2.md");
  std::string result = std::get<1>(ast.accept(visitor));

  ASSERT_EQ(expected, result);
};

TEST_F(AstTests, PassthroughAstPassesValueThrough) {
  Frizz::PassthroughAst ast("foo");
  std::string val = std::get<1>(ast.accept(visitor));

  ASSERT_EQ(val, "foo");
}