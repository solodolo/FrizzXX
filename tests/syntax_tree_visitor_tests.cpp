/*
 * syntax_tree_visitor_tests.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: dmmettlach
 */
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

#include "abstract_syntax_trees.h"
#include "test_helpers.h"

class SytaxTreeVisitorTests : public ::testing::Test {
protected:
  Frizz::FrizzConfig config;
  Frizz::FileUtility util;
  Frizz::AstVisitor visitor;
  Frizz::FileContentVisitor fc_visitor;

  SytaxTreeVisitorTests()
    : util(config)
    , fc_visitor(util) {
    config.set_parent_dir(FrizzTesting::find_path("tests/test_files"));
  }
};

TEST_F(SytaxTreeVisitorTests, AssignmentAstGetsFileName) {
  std::string expected = "test2.md";
  Frizz::AssignmentAst ast("use", expected);
  std::string result = std::get<1>(ast.accept(visitor));

  ASSERT_EQ(expected, result);
};

TEST_F(SytaxTreeVisitorTests, PassthroughAstPassesValueThrough) {
  Frizz::PassthroughAst ast("foo");
  std::string val = std::get<1>(ast.accept(visitor));

  ASSERT_EQ(val, "foo");
}

TEST_F(SytaxTreeVisitorTests, FileContentVisitorGetsFileContents) {
  std::string expected = FrizzTesting::read_file("tests/test_files/partials/test1.md");
  Frizz::AssignmentAst ast("src", "test1.md");
  std::string result = ast.accept(fc_visitor);

  ASSERT_EQ(expected, result);
}

TEST_F(SytaxTreeVisitorTests, AstVisitorDoesNotReturnSrcLiteral) {
  Frizz::AssignmentAst ast("src", "blah.md");
  std::tuple<std::string, std::string> results = ast.accept(visitor);

  EXPECT_EQ(std::get<0>(results), "");
  EXPECT_EQ(std::get<1>(results), "");
}