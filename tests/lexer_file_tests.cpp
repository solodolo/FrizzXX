/*
 * lexer_file_tests.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: dmmettlach
 */
#include <gtest/gtest.h>

#include "lexer.h"
#include "test_helpers.h"

class LexerFileTests : public ::testing::Test {
protected:
  Frizz::Lexer lexer;
  void SetUp() override { this->lexer.clear_tokens(); }
};

// context replacement
TEST_F(LexerFileTests, ContextReplacementFromFile) {
  std::filesystem::path filepath = Frizz::find_path("tests/test_files/partials/contextual_partial.md");
  // std::filesystem::path filepath("./tests/test_files/partials/contextual_partial.md");
  lexer.lex(filepath);

  std::vector<Frizz::Token> tokens = lexer.get_tokens();

  ASSERT_EQ(tokens.size(), 9);

  EXPECT_EQ(tokens[0].value, "[");
  EXPECT_EQ(tokens[0].id, Frizz::TokType::tok_sym);

  EXPECT_EQ(tokens[1].value, "post");
  EXPECT_EQ(tokens[1].id, Frizz::TokType::tok_ctx_name);

  EXPECT_EQ(tokens[2].value, "title");
  EXPECT_EQ(tokens[2].id, Frizz::TokType::tok_ctx_val);

  EXPECT_EQ(tokens[3].value, "]");
  EXPECT_EQ(tokens[3].id, Frizz::TokType::tok_sym);

  EXPECT_EQ(tokens[4].value, "(");
  EXPECT_EQ(tokens[4].id, Frizz::TokType::tok_sym);

  EXPECT_EQ(tokens[5].value, "test_link");
  EXPECT_EQ(tokens[5].id, Frizz::TokType::tok_ident);

  EXPECT_EQ(tokens[6].value, ")");
  EXPECT_EQ(tokens[6].id, Frizz::TokType::tok_sym);

  EXPECT_EQ(tokens[7].value, "\n");
  EXPECT_EQ(tokens[7].id, Frizz::TokType::tok_ws);

  EXPECT_EQ(tokens[8].value, "");
  EXPECT_EQ(tokens[8].id, Frizz::TokType::tok_eof);
}

TEST_F(LexerFileTests, FromFileTest) {
  std::filesystem::path input_path = Frizz::find_path("tests/test_files/partials/test1.md");
  lexer.lex(input_path);

  auto tokens = lexer.get_tokens();

  ASSERT_EQ(tokens.size(), 45);
  EXPECT_EQ(tokens.back().id, Frizz::TokType::tok_eof);
};

TEST_F(LexerFileTests, ForLoopTest) {
  std::filesystem::path input_path = Frizz::find_path("tests/test_files/sources/for_loop.md");
  lexer.lex(input_path);

  auto tokens = lexer.get_tokens();
  std::vector<Frizz::TokType> expected = {
    Frizz::TokType::tok_block, Frizz::TokType::tok_ws,    Frizz::TokType::tok_for,
    Frizz::TokType::tok_ws,    Frizz::TokType::tok_ident, Frizz::TokType::tok_ws,
    Frizz::TokType::tok_in,    Frizz::TokType::tok_ws,    Frizz::TokType::tok_str,
    Frizz::TokType::tok_ws,    Frizz::TokType::tok_block, Frizz::TokType::tok_ws,
    Frizz::TokType::tok_ident, Frizz::TokType::tok_sym,   Frizz::TokType::tok_str,
    Frizz::TokType::tok_ws,    Frizz::TokType::tok_eof
  };

  ASSERT_EQ(tokens.size(), expected.size());
  for(int i = 0; i < tokens.size(); ++i) {
    EXPECT_EQ(tokens[i].id, expected[i]) << "Failure on token " << i;
  }
}