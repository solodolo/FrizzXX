/*
 * parser.h
 *
 *  Created on: Aug 11, 2020
 *      Author: dmmettlach
 */

#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

#include <memory>

#include "lexer.h"
#include "structures.h"

namespace Frizz {
class Parser {
public:
	void next_token();
	void set_tokens(std::vector<Token> tokens);
	void parse();
	const std::vector<std::unique_ptr<BasicStruct>>& get_structures();
	void clear_structures();

private:
	Token cur_tok;
	std::string last_val;
	std::vector<Token> tokens;
	std::vector<std::string> errors;
	std::vector<std::unique_ptr<BasicStruct>> structures;

private:
	std::unique_ptr<BasicStruct> block();
	std::unique_ptr<BasicStruct> passthrough();
	
	void preamble();
	bool peek_current(TokType id);
	bool optional_found(TokType id);
	bool optional_found(TokType id, std::string val);
	bool required_found(TokType id);
	bool required_found(TokType id, std::string val);
	void add_error(std::string message);
	bool has_errors();
};
}
#endif /* SRC_PARSER_H_ */
