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
  Frizz::FileContentVisitor fc_visitor;

  AstTests()
    : util(config)
    , fc_visitor(util) {
    config.load_configuration("./tests/test_files/config/test2.json");
  }
};

TEST_F(AstTests, AssignmentAstGetsFileName) {
  std::string expected = "test2.md";
  Frizz::AssignmentAst ast("src", expected);
  std::string result = std::get<1>(ast.accept(visitor));

  ASSERT_EQ(expected, result);
};

TEST_F(AstTests, PassthroughAstPassesValueThrough) {
  Frizz::PassthroughAst ast("foo");
  std::string val = std::get<1>(ast.accept(visitor));

  ASSERT_EQ(val, "foo");
}

TEST_F(AstTests, FileContentVisitorGetsFileContents) {
  std::string expected = get_expected("./tests/test_files/partials/test1.md");
  Frizz::AssignmentAst ast("src", "test1.md");
  std::string result = ast.accept(fc_visitor);

  ASSERT_EQ(expected, result);
}