/*
 * Frizz++_tests.cpp
 *
 *  Created on: Aug 9, 2020
 *      Author: dmmettlach
 */
#include <gtest/gtest.h>

#include "lexer.h"

class LexerTests : public ::testing::Test {
protected:
	void SetUp() override {
		lexer.tokens.clear();
	}

	Frizz::Lexer lexer;
};

// BLOCK @@
TEST_F(LexerTests, BlockTok) {
	lexer.set_line("@@");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_block);
}

TEST_F(LexerTests, HalfBlockTok) {
	lexer.set_line("@");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 0);
}

TEST_F(LexerTests, LeadingWhitespaceBlockTok) {
	lexer.set_line("       @@");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_block);
}

TEST_F(LexerTests, MiddleWhitespaceBlockTok) {
	lexer.set_line("@@     @@");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 2);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_block);
	EXPECT_EQ(lexer.tokens[1].id, Frizz::TokType::tok_block);
}

TEST_F(LexerTests, MultiSequenceBlockTok) {
	lexer.set_line("@@@@");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 2);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_block);
	EXPECT_EQ(lexer.tokens[1].id, Frizz::TokType::tok_block);
}

// PREAMBLE ~~
TEST_F(LexerTests, PreambleTok) {
	lexer.set_line("~~\n");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
}

TEST_F(LexerTests, SingleLinePreambleTok) {
	lexer.set_line("~~");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 0);
}

TEST_F(LexerTests, SpacePreambleTok) {
	lexer.set_line("~~ ");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 0);
}



// STRING "blah"
TEST_F(LexerTests, StrTok) {
	lexer.set_line("\"a\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_str);
	EXPECT_EQ(lexer.tokens[0].value, "a");
}

TEST_F(LexerTests, HelloWordStrTok) {
	lexer.set_line("\"hello world\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_str);
	EXPECT_EQ(lexer.tokens[0].value, "hello world");
}

TEST_F(LexerTests, NumberStrTok) {
	lexer.set_line("\"587.22\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_str);
	EXPECT_EQ(lexer.tokens[0].value, "587.22");
}

TEST_F(LexerTests, BodyStrTok) {
	lexer.set_line("\"@@\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_str);
	EXPECT_EQ(lexer.tokens[0].value, "@@");
}

TEST_F(LexerTests, MultiStrTok) {
	lexer.set_line("\"a\"    \"b\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 2);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_str);
	EXPECT_EQ(lexer.tokens[1].id, Frizz::TokType::tok_str);
	EXPECT_EQ(lexer.tokens[0].value, "a");
	EXPECT_EQ(lexer.tokens[1].value, "b");
}

TEST_F(LexerTests, EmptyStrTok) {
	lexer.set_line("\"\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 0);
}

TEST_F(LexerTests, NewLineStrTok) {
	lexer.set_line("\"\n\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 0);
}

//IDENTIFIER foo or foo=
TEST_F(LexerTests, IdentTok) {
	lexer.set_line("src=");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
	EXPECT_EQ(lexer.tokens[0].value, "src");
}

TEST_F(LexerTests, ExpressionIdentTok) {
	lexer.set_line("foo=\"bar\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 2);
	EXPECT_EQ(lexer.tokens[0].value, "foo");
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_ident);

	EXPECT_EQ(lexer.tokens[1].value, "bar");
	EXPECT_EQ(lexer.tokens[1].id, Frizz::TokType::tok_str);
}

TEST_F(LexerTests, DoubleIdentTok) {
	lexer.set_line("file=value");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 2);
	EXPECT_EQ(lexer.tokens[0].value, "file");
	EXPECT_EQ(lexer.tokens[1].value, "value");
}

TEST_F(LexerTests, NoEqualsIdentTok) {
	lexer.set_line("foo");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
	EXPECT_EQ(lexer.tokens[0].value, "foo");
}

//FOR for
TEST_F(LexerTests, ForTok) {
	lexer.set_line("for ");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_for);
}

//IN in
TEST_F(LexerTests, InTok) {
	lexer.set_line("in ");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 1);
	EXPECT_EQ(lexer.tokens[0].id, Frizz::TokType::tok_in);
}

//DEFAULT when there is no match
TEST_F(LexerTests, NoMatch) {
	lexer.set_line("# markdown header");
	lexer.next_tok();
	ASSERT_EQ(lexer.tokens.size(), 0);
}
