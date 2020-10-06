/*
 * structure_tests.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: dmmettlach
 */

#include <gtest/gtest.h>

#include "abstract_syntax_trees.h"

class AstTests : public ::testing::Test {
protected:

  AstTests() {};
};

TEST_F(AstTests, AssignmentAstIsSrcReturnsTrueWhenNamedSrc) {
  Frizz::AssignmentAst ast("src", "bar");
  ASSERT_TRUE(ast.is_src());
}

TEST_F(AstTests, AssignmentAstIsSrcReturnsFalseWhenNotNamedSrc) {
  Frizz::AssignmentAst ast("foo", "bar");
  ASSERT_FALSE(ast.is_src());
}