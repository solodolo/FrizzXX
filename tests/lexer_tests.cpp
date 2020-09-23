/*
 * lexer_tests.cpp
 *
 *  Created on: Aug 9, 2020
 *      Author: dmmettlach
 */
#include <gtest/gtest.h>

#include "lexer.h"
#include "test_helpers.h"

class LexerTests : public ::testing::Test {
protected:
  Frizz::Lexer lexer;
  void SetUp() override { this->lexer.clear_tokens(); }
};

// BLOCK @@
TEST_F(LexerTests, BlockTok) {
  lexer.set_line("@@");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 1);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_block);
}

TEST_F(LexerTests, HalfBlockTok) {
  lexer.set_line("@");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 1);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_sym);
}

TEST_F(LexerTests, LeadingWhitespaceBlockTok) {
  lexer.set_line("       @@");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 2);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ws);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_block);
}

TEST_F(LexerTests, MiddleWhitespaceBlockTok) {
  lexer.set_line("@@     @@");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 3);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_block);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_ws);
  EXPECT_EQ(lexer.get_tokens()[2].id, Frizz::TokType::tok_block);
}

TEST_F(LexerTests, MultiSequenceBlockTok) {
  lexer.set_line("@@@@");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 2);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_block);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_block);
}

// PREAMBLE ~~
TEST_F(LexerTests, PreambleTok) {
  lexer.set_line("~~");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 1);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_preamble);
}

TEST_F(LexerTests, SingleLinePreambleTok) {
  lexer.set_line("~~\n");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 3);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_sym);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_sym);
  EXPECT_EQ(lexer.get_tokens()[2].id, Frizz::TokType::tok_ws);
}

TEST_F(LexerTests, SpacePreambleTok) {
  lexer.set_line("~~ ");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 3);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_sym);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_sym);
  EXPECT_EQ(lexer.get_tokens()[2].id, Frizz::TokType::tok_ws);
}

// CTX {post.title}
TEST_F(LexerTests, ContextTok) {
  lexer.set_line("{post.title}");
  lexer.next_tok();

  ASSERT_EQ(lexer.get_tokens().size(), 2);
  EXPECT_EQ(lexer.get_tokens()[0].value, "post");
  EXPECT_EQ(lexer.get_tokens()[1].value, "title");
}

TEST_F(LexerTests, ContextNoNamespaceTok) {
  lexer.set_line("{.foo}");
  lexer.next_tok();

  ASSERT_EQ(lexer.get_tokens().size(), 2);
  EXPECT_EQ(lexer.get_tokens()[0].value, "");
  EXPECT_EQ(lexer.get_tokens()[1].value, "foo");
}

// STRING "blah"
TEST_F(LexerTests, StrTok) {
  lexer.set_line("\"a\"");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 1);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_str);
  EXPECT_EQ(lexer.get_tokens()[0].value, "a");
}

TEST_F(LexerTests, HelloWordStrTok) {
  lexer.set_line("\"hello world\"");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 1);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_str);
  EXPECT_EQ(lexer.get_tokens()[0].value, "hello world");
}

TEST_F(LexerTests, NumberStrTok) {
  lexer.set_line("\"587.22\"");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 1);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_str);
  EXPECT_EQ(lexer.get_tokens()[0].value, "587.22");
}

TEST_F(LexerTests, BodyStrTok) {
  lexer.set_line("\"@@\"");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 1);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_str);
  EXPECT_EQ(lexer.get_tokens()[0].value, "@@");
}

TEST_F(LexerTests, MultiStrTok) {
  lexer.set_line("\"a\"    \"b\"");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 3);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_str);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_ws);
  EXPECT_EQ(lexer.get_tokens()[2].id, Frizz::TokType::tok_str);
  EXPECT_EQ(lexer.get_tokens()[0].value, "a");
  EXPECT_EQ(lexer.get_tokens()[2].value, "b");
}

TEST_F(LexerTests, EmptyStrTok) {
  lexer.set_line("\"\"");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 2);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_sym);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_sym);
}

TEST_F(LexerTests, NewLineStrTok) {
  lexer.set_line("\"\n\"");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 3);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_sym);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_ws);
  EXPECT_EQ(lexer.get_tokens()[2].id, Frizz::TokType::tok_sym);
}

// IDENTIFIER foo or foo=
TEST_F(LexerTests, IdentTok) {
  lexer.set_line("src=");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 2);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);
  EXPECT_EQ(lexer.get_tokens()[0].value, "src");
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_sym);
  EXPECT_EQ(lexer.get_tokens()[1].value, "=");
}

TEST_F(LexerTests, ExpressionIdentTok) {
  lexer.set_line("foo=\"bar\"");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 3);
  EXPECT_EQ(lexer.get_tokens()[0].value, "foo");
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);

  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_sym);
  EXPECT_EQ(lexer.get_tokens()[1].value, "=");

  EXPECT_EQ(lexer.get_tokens()[2].value, "bar");
  EXPECT_EQ(lexer.get_tokens()[2].id, Frizz::TokType::tok_str);
}

TEST_F(LexerTests, DoubleIdentTok) {
  lexer.set_line("file=value");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 3);
  EXPECT_EQ(lexer.get_tokens()[0].value, "file");
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);

  EXPECT_EQ(lexer.get_tokens()[1].value, "=");
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_sym);

  EXPECT_EQ(lexer.get_tokens()[2].value, "value");
  EXPECT_EQ(lexer.get_tokens()[2].id, Frizz::TokType::tok_ident);
}

TEST_F(LexerTests, NoEqualsIdentTok) {
  lexer.set_line("foo");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 1);
  EXPECT_EQ(lexer.get_tokens()[0].value, "foo");
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);
}

TEST_F(LexerTests, FortuneIdentTok) {
  lexer.set_line("fortune");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 2);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_for);
  EXPECT_EQ(lexer.get_tokens()[0].value, "for");
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_ident);
  EXPECT_EQ(lexer.get_tokens()[1].value, "tune");
}

TEST_F(LexerTests, ForWithoutSpaceIdentTok) {
  lexer.set_line("for");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 1);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_for);
  EXPECT_EQ(lexer.get_tokens()[0].value, "for");
}

TEST_F(LexerTests, InWithoutSpaceIdentTok) {
  lexer.set_line("in");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 1);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_in);
  EXPECT_EQ(lexer.get_tokens()[0].value, "in");
}

// FOR for
TEST_F(LexerTests, ForTok) {
  lexer.set_line("for ");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 2);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_for);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_ws);
}

// IN in
TEST_F(LexerTests, InTok) {
  lexer.set_line("in ");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 2);
  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_in);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_ws);
}

// DEFAULT when there is no match
TEST_F(LexerTests, NoMatchAtStart) {
  lexer.set_line("# markdown header");
  lexer.next_tok();
  ASSERT_EQ(lexer.get_tokens().size(), 5);

  EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_sym);
  EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_ws);
  EXPECT_EQ(lexer.get_tokens()[2].id, Frizz::TokType::tok_ident);
  EXPECT_EQ(lexer.get_tokens()[3].id, Frizz::TokType::tok_ws);
  EXPECT_EQ(lexer.get_tokens()[4].id, Frizz::TokType::tok_ident);
}

// Complete example

// helper for complete test
std::vector<Frizz::TokType> map_tokens_by_id(std::vector<Frizz::Token> tokens) {
  std::vector<Frizz::TokType> by_id;

  for(std::vector<Frizz::Token>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
    by_id.push_back(it->id);
  }

  return by_id;
}

// context replacement
TEST_F(LexerTests, ContextReplacement) {
  lexer.set_line("[{post.title}](test_link)");
  lexer.next_tok();

  std::vector<Frizz::Token> tokens = lexer.get_tokens();

  ASSERT_EQ(tokens.size(), 7);

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
}

// context replacement
TEST_F(LexerTests, ContextReplacementFromFile) {
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

TEST_F(LexerTests, Complete) {
  std::vector<std::string> lines;
  lines.push_back("# Title");
  lines.push_back("~~");
  lines.push_back("@@ foo=\"bar\"");
  lines.push_back("~~");
  lines.push_back("");
  lines.push_back("@@ src=\"path/to/file.md\"");
  lines.push_back("");
  lines.push_back("**bold text**");
  lines.push_back("");
  lines.push_back("@@ for foo in \"path/to/bar\"");
  lines.push_back("@@ src=\"preview.md\"");

  std::vector<std::vector<Frizz::TokType>> expected;
  expected.push_back(std::vector<Frizz::TokType> {
    Frizz::TokType::tok_sym, Frizz::TokType::tok_ws, Frizz::TokType::tok_ident });

  expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_preamble });

  expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_block,
                                                   Frizz::TokType::tok_ws,
                                                   Frizz::TokType::tok_ident,
                                                   Frizz::TokType::tok_sym,
                                                   Frizz::TokType::tok_str });

  expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_preamble });

  expected.push_back(std::vector<Frizz::TokType>());

  expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_block,
                                                   Frizz::TokType::tok_ws,
                                                   Frizz::TokType::tok_ident,
                                                   Frizz::TokType::tok_sym,
                                                   Frizz::TokType::tok_str });

  expected.push_back(std::vector<Frizz::TokType>());

  expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_sym,
                                                   Frizz::TokType::tok_sym,
                                                   Frizz::TokType::tok_ident,
                                                   Frizz::TokType::tok_ws,
                                                   Frizz::TokType::tok_ident,
                                                   Frizz::TokType::tok_sym,
                                                   Frizz::TokType::tok_sym });

  expected.push_back(std::vector<Frizz::TokType>());

  expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_block,
                                                   Frizz::TokType::tok_ws,
                                                   Frizz::TokType::tok_for,
                                                   Frizz::TokType::tok_ws,
                                                   Frizz::TokType::tok_ident,
                                                   Frizz::TokType::tok_ws,
                                                   Frizz::TokType::tok_in,
                                                   Frizz::TokType::tok_ws,
                                                   Frizz::TokType::tok_str });

  expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_block,
                                                   Frizz::TokType::tok_ws,
                                                   Frizz::TokType::tok_ident,
                                                   Frizz::TokType::tok_sym,
                                                   Frizz::TokType::tok_str });

  for(int i = 0; i < lines.size(); ++i) {
    lexer.set_line(lines[i]);
    lexer.next_tok();
    std::vector<Frizz::TokType> line_expected = expected[i];
    auto tokens = lexer.get_tokens();
    EXPECT_EQ(line_expected, map_tokens_by_id(tokens)) << "Failure on line " << i;

    lexer.clear_tokens();
  }
}

// read file
TEST_F(LexerTests, FromFileTest) {
  std::filesystem::path input_path = Frizz::find_path("tests/test_files/partials/test1.md");
  lexer.lex(input_path);

  auto tokens = lexer.get_tokens();

  ASSERT_EQ(tokens.size(), 45);
  EXPECT_EQ(tokens.back().id, Frizz::TokType::tok_eof);
};

TEST_F(LexerTests, ForLoopTest) {
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