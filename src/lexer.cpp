/*
 * lexer.cpp
 *
 *  Created on: Aug 8, 2020
 *      Author: dmmettlach
 */
#include <regex>
#include <iostream>

#include "lexer.h"

bool Frizz::Lexer::tok_is_a(TokType type) {
	return this->cur_tok.id == type;
}

void Frizz::Lexer::next_tok() {
	std::string cur_line = this->line;

	std::regex whitespace("^\\s+");
	std::regex block_pattern("^@@");
	std::regex preamble_pattern("^~~\n");
	std::regex str_pattern("^\"[^\n]+?\"");
	std::regex ident_pattern("^[a-zA-Z_-]+=?");
	std::regex for_pattern("^for ");
	std::regex in_pattern("^in ");
	std::smatch results;

	this->tokens.clear();

	while(!cur_line.empty()) {
		results = std::smatch();

		if(std::regex_search(cur_line, results, whitespace)) {
			//check for whitespace
		}
		else if(std::regex_search(cur_line, results, block_pattern)) {
			Token tok(tok_block);
			this->tokens.push_back(tok);
		}
		else if(std::regex_search(cur_line, results, preamble_pattern)) {
			Token tok(tok_preamble);
			this->tokens.push_back(tok);
		}
		else if(std::regex_search(cur_line, results, for_pattern)) {
			Token tok(tok_for);
			this->tokens.push_back(tok);
		}
		else if(std::regex_search(cur_line, results, in_pattern)) {
			Token tok(tok_in);
			this->tokens.push_back(tok);
		}
		else if(std::regex_search(cur_line, results, str_pattern)) {
			Token tok(tok_str);

			//get the contents of the string witout the dbl quotes
			std::string result_str = results.str();
			tok.value = result_str.substr(1, result_str.length() - 2);

			this->tokens.push_back(tok);
		}
		else if(std::regex_search(cur_line, results, ident_pattern)) {
			Token tok(tok_ident);

			//get the name of the identifier without '='
			std::string result_str = results.str();

			if(result_str[result_str.length() - 1] == '=') {
				tok.value = result_str.substr(0, result_str.length() - 1);
			}
			else {
				tok.value = result_str;
			}

			this->tokens.push_back(tok);
		}
		else {
			//no match so discard the first character and try again
			cur_line = cur_line.substr(1);
		}

		if(!results.empty()) {
			cur_line.erase(results.position(0), results.length(0));
		}
	}
}

void Frizz::Lexer::set_line(std::string line) {
	this->line = line;
}

void Frizz::Lexer::print_tokens() {
	std::cout << "Current tokens: ";

	for(std::vector<Token>::iterator it = this->tokens.begin(); it != this->tokens.end(); ++it) {
		std::cout << it->to_string() << ", ";
	}

	std::cout << std::endl;
}
