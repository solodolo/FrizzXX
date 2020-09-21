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
#include "test_helpers.h"

std::string get_expected(std::filesystem::path path) {
  std::ifstream input(Frizz::find_path(path));
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
    config.set_parent_dir(Frizz::find_path("tests/test_files"));
  }
};

TEST_F(AstTests, AssignmentAstGetsFileName) {
  std::string expected = "test2.md";
  Frizz::AssignmentAst ast("use", expected);
  std::string result = std::get<1>(ast.accept(visitor));

  ASSERT_EQ(expected, result);
};

TEST_F(AstTests, PassthroughAstPassesValueThrough) {
  Frizz::PassthroughAst ast("foo");
  std::string val = std::get<1>(ast.accept(visitor));

  ASSERT_EQ(val, "foo");
}

TEST_F(AstTests, FileContentVisitorGetsFileContents) {
  std::string expected = get_expected("tests/test_files/partials/test1.md");
  Frizz::AssignmentAst ast("src", "test1.md");
  std::string result = ast.accept(fc_visitor);

  ASSERT_EQ(expected, result);
}

TEST_F(AstTests, AstVisitorDoesNotReturnSrcLiteral) {
  Frizz::AssignmentAst ast("src", "blah.md");
  std::tuple<std::string, std::string> results = ast.accept(visitor);

  EXPECT_EQ(std::get<0>(results), "");
  EXPECT_EQ(std::get<1>(results), "");
}