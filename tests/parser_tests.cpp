/*
 * Frizz++_tests.cpp
 *
 *  Created on: Aug 9, 2020
 *      Author: dmmettlach
 */
#include <gtest/gtest.h>

#include "parser.h"

using namespace Frizz;

TEST(ParserTests, SingleLineInputs) {
	Parser parser;

	std::vector<Token> tokens { Token(TokType::tok_block), Token(
			TokType::tok_ident, "src"), Token(TokType::tok_sym, "="), Token(
			TokType::tok_str, "test.md") };

	parser.set_tokens(tokens);

	parser.parse();
}
