/*
 * parser.h
 *
 *  Created on: Aug 11, 2020
 *      Author: dmmettlach
 */

#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

#include "lexer.h"

namespace Frizz {
class Parser {
public:
	void next_token();
	void set_tokens(std::vector<Token> tokens);

private:
	Token cur_tok;
	std::vector<Token> tokens;
	std::vector<std::string> errors;

private:
	void block();
	void preamble();
	bool optional_found(TokType id);
	bool required_found(TokType id);
	bool required_found(TokType id, std::string val);
	void add_error(std::string message);
};
}

#endif /* SRC_PARSER_H_ */
