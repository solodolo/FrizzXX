/*
 * lexer.h
 *
 *  Created on: Aug 8, 2020
 *      Author: dmmettlach
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <string>
#include <vector>

namespace Frizz {
enum TokType {
	tok_none = -1,
	tok_block = 1,
	tok_preamble = 2,
	tok_ident = 3,
	tok_str = 4,
	tok_src = 5,
	tok_for = 6,
	tok_in = 7
};

struct Token {
	const TokType id;
	Token(TokType id) :
			id(id) {
	}

	std::string value;

	std::string to_string() {
		return std::to_string(id);
	}
};

class Lexer {
public:
	Token cur_tok;
	std::vector<Token> tokens;
public:
	Lexer(): cur_tok(tok_none) {};
	void next_tok();
	void set_line(std::string line);
	bool tok_is_a(TokType type);

private:
	std::string line;

private:
	void print_tokens();
};
}

#endif /* LEXER_H_ */
