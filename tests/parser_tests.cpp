/*
 * parser_tests.cpp
 *
 *  Created on: Aug 9, 2020
 *      Author: dmmettlach
 */
#include <gtest/gtest.h>

#include "parser.h"

using namespace Frizz;

class ParserTests : public ::testing::Test {
public:
  ParserTests()
    : util(config)
    , parser(util) {}
  FrizzConfig config;
  FileUtility util;
  Parser parser;
};

TEST_F(ParserTests, SingleLineSingleExp) {
  std::vector<Token> tokens { Token(TokType::tok_block),          Token(TokType::tok_ws),
                              Token(TokType::tok_ident, "src"),   Token(TokType::tok_sym, "="),
                              Token(TokType::tok_str, "test.md"), Token(TokType::tok_nl) };

  parser.set_tokens(tokens);

  parser.parse();

  ASSERT_EQ(parser.get_trees().size(), 1);
  EXPECT_EQ(parser.get_trees()[0]->get_value(), "test.md");
}

TEST_F(ParserTests, SingleLineMultiExp) {
  std::vector<Token> tokens { Token(TokType::tok_block),           Token(TokType::tok_ws),
                              Token(TokType::tok_ident, "src"),    Token(TokType::tok_sym, "="),
                              Token(TokType::tok_str, "test1.md"), Token(TokType::tok_sym, ","),
                              Token(TokType::tok_block),           Token(TokType::tok_ws),
                              Token(TokType::tok_ident, "src"),    Token(TokType::tok_sym, "="),
                              Token(TokType::tok_str, "test2.md") };

  parser.set_tokens(tokens);

  parser.parse();

  ASSERT_EQ(parser.get_trees().size(), 2);
  EXPECT_EQ(parser.get_trees()[0]->get_value(), "test1.md");
  EXPECT_EQ(parser.get_trees()[1]->get_value(), "test2.md");
}

TEST_F(ParserTests, SinglePassthroughLine) {
  std::vector<Token> tokens { Token(TokType::tok_sym, "#"),
                              Token(TokType::tok_ident, "this is a header") };

  parser.set_tokens(tokens);

  parser.parse();

  ASSERT_EQ(parser.get_trees().size(), 2);
  EXPECT_EQ(parser.get_trees()[0]->get_value(), "#");
  EXPECT_EQ(parser.get_trees()[1]->get_value(), "this is a header");
}

TEST_F(ParserTests, PreambleSingleAssignment) {
  std::vector<Token> tokens { Token(TokType::tok_preamble),
                              Token(TokType::tok_ident),
                              Token(TokType::tok_sym, "="),
                              Token(TokType::tok_str, "foo"),
                              Token(TokType::tok_preamble) };

  parser.set_tokens(tokens);

  parser.parse();

  ASSERT_EQ(parser.get_trees().size(), 1);
  EXPECT_EQ(parser.get_trees()[0]->get_value(), "foo");
}

TEST_F(ParserTests, PreambleMultiAssignment) {
  std::vector<Token> tokens { Token(TokType::tok_preamble),   Token(TokType::tok_ident, "a"),
                              Token(TokType::tok_sym, "="),   Token(TokType::tok_str, "foo"),
                              Token(TokType::tok_ident, "b"), Token(TokType::tok_sym, "="),
                              Token(TokType::tok_str, "bar"), Token(TokType::tok_ident, "c"),
                              Token(TokType::tok_sym, "="),   Token(TokType::tok_str, "baz"),
                              Token(TokType::tok_preamble) };

  parser.set_tokens(tokens);

  parser.parse();

  ASSERT_EQ(parser.get_trees().size(), 3);
  EXPECT_EQ(parser.get_trees()[0]->get_value(), "foo");
  EXPECT_EQ(parser.get_trees()[1]->get_value(), "bar");
  EXPECT_EQ(parser.get_trees()[2]->get_value(), "baz");
}

TEST_F(ParserTests, ForLoopContext) {
  config.load_configuration("./tests/test_files/config/test2.json");

  std::vector<Token> tokens { Token(TokType::tok_block),
                              Token(TokType::tok_ws),
                              Token(TokType::tok_for),
                              Token(TokType::tok_ws),
                              Token(TokType::tok_ident),
                              Token(TokType::tok_ws),
                              Token(TokType::tok_in),
                              Token(TokType::tok_ws),
                              Token(TokType::tok_str, "posts"),
                              Token(TokType::tok_nl),
                              Token(TokType::tok_block),
                              Token(TokType::tok_ws),
                              Token(TokType::tok_ident),
                              Token(TokType::tok_sym, "="),
                              Token(TokType::tok_str, "test.md") };

  parser.set_tokens(tokens);

  parser.parse();

  auto trees = parser.get_trees();
  ASSERT_EQ(trees.size(), 1);

  Frizz::ContextChildrenVisitor visitor;
  auto children = trees[0]->accept(visitor);

  ASSERT_EQ(children.size(), 3);
}
