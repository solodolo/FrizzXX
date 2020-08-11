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
	Frizz::Lexer lexer;
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
	ASSERT_EQ(lexer.get_tokens().size(), 0);
}

TEST_F(LexerTests, LeadingWhitespaceBlockTok) {
	lexer.set_line("       @@");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 1);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_block);
}

TEST_F(LexerTests, MiddleWhitespaceBlockTok) {
	lexer.set_line("@@     @@");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 2);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_block);
	EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_block);
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
	lexer.set_line("~~\n");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 1);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_preamble);
}

TEST_F(LexerTests, SingleLinePreambleTok) {
	lexer.set_line("~~");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 0);
}

TEST_F(LexerTests, SpacePreambleTok) {
	lexer.set_line("~~ ");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 0);
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
	ASSERT_EQ(lexer.get_tokens().size(), 2);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_str);
	EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_str);
	EXPECT_EQ(lexer.get_tokens()[0].value, "a");
	EXPECT_EQ(lexer.get_tokens()[1].value, "b");
}

TEST_F(LexerTests, EmptyStrTok) {
	lexer.set_line("\"\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 0);
}

TEST_F(LexerTests, NewLineStrTok) {
	lexer.set_line("\"\n\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 0);
}

//IDENTIFIER foo or foo=
TEST_F(LexerTests, IdentTok) {
	lexer.set_line("src=");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 1);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);
	EXPECT_EQ(lexer.get_tokens()[0].value, "src");
}

TEST_F(LexerTests, ExpressionIdentTok) {
	lexer.set_line("foo=\"bar\"");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 2);
	EXPECT_EQ(lexer.get_tokens()[0].value, "foo");
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);

	EXPECT_EQ(lexer.get_tokens()[1].value, "bar");
	EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_str);
}

TEST_F(LexerTests, DoubleIdentTok) {
	lexer.set_line("file=value");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 2);
	EXPECT_EQ(lexer.get_tokens()[0].value, "file");
	EXPECT_EQ(lexer.get_tokens()[1].value, "value");

	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);
	EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_ident);
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
	ASSERT_EQ(lexer.get_tokens().size(), 1);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);
	EXPECT_EQ(lexer.get_tokens()[0].value, "fortune");
}

TEST_F(LexerTests, ForWithoutSpaceIdentTok) {
	lexer.set_line("for");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 1);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);
	EXPECT_EQ(lexer.get_tokens()[0].value, "for");
}

TEST_F(LexerTests, InWithoutSpaceIdentTok) {
	lexer.set_line("in");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 1);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);
	EXPECT_EQ(lexer.get_tokens()[0].value, "in");
}

//FOR for
TEST_F(LexerTests, ForTok) {
	lexer.set_line("for ");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 1);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_for);
}

//IN in
TEST_F(LexerTests, InTok) {
	lexer.set_line("in ");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 1);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_in);
}

//DEFAULT when there is no match
TEST_F(LexerTests, NoMatchAtStart) {
	lexer.set_line("# markdown header");
	lexer.next_tok();
	ASSERT_EQ(lexer.get_tokens().size(), 2);
	EXPECT_EQ(lexer.get_tokens()[0].id, Frizz::TokType::tok_ident);
	EXPECT_EQ(lexer.get_tokens()[1].id, Frizz::TokType::tok_ident);
}


//Complete example

//helper for complete test
std::vector<Frizz::TokType> map_tokens_by_id(std::vector<Frizz::Token> tokens) {
	std::vector<Frizz::TokType> by_id;

	for(std::vector<Frizz::Token>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
		by_id.push_back(it->id);
	}

	return by_id;
}
TEST_F(LexerTests, Complete) {
	std::vector<std::string> lines;
	lines.push_back("# This is a markdown file");
	lines.push_back("~~");
	lines.push_back("@@ foo=\"bar\"");
	lines.push_back("~~");
	lines.push_back("");
	lines.push_back("@@ src=\"path/to/file.md\"");
	lines.push_back("");
	lines.push_back("**here is some bold text**");
	lines.push_back("");
	lines.push_back("@@ for foo in \"path/to/bar\"");
	lines.push_back("@@ src=\"preview.md\" args=file");

	std::vector<std::vector<Frizz::TokType>> expected;
	expected.push_back(
			std::vector<Frizz::TokType>(5, Frizz::TokType::tok_ident));
	expected.push_back(std::vector<Frizz::TokType> {
			Frizz::TokType::tok_preamble });
	expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_block,
			Frizz::TokType::tok_ident, Frizz::TokType::tok_str });
	expected.push_back(std::vector<Frizz::TokType> {
			Frizz::TokType::tok_preamble });
	expected.push_back(std::vector<Frizz::TokType>());
	expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_block,
			Frizz::TokType::tok_ident, Frizz::TokType::tok_str });
	expected.push_back(std::vector<Frizz::TokType>());
	expected.push_back(
			std::vector<Frizz::TokType>(5, Frizz::TokType::tok_ident));
	expected.push_back(std::vector<Frizz::TokType>());
	expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_block,
			Frizz::TokType::tok_for, Frizz::TokType::tok_ident,
			Frizz::TokType::tok_in, Frizz::TokType::tok_str });
	expected.push_back(std::vector<Frizz::TokType> { Frizz::TokType::tok_block,
			Frizz::TokType::tok_ident, Frizz::TokType::tok_str,
			Frizz::TokType::tok_ident, Frizz::TokType::tok_ident });

	for(int i = 0; i < lines.size(); ++i) {
		lexer.set_line(lines[i] + "\n");
		lexer.next_tok();
		std::vector<Frizz::TokType> line_expected = expected[i];
		EXPECT_EQ(line_expected, map_tokens_by_id(lexer.get_tokens()));

	}
}
