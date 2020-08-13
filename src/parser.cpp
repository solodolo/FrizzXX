/*
 * parser.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: dmmettlach
 */

#include "parser.h"

void Frizz::Parser::set_tokens(std::vector<Token> tokens) {
	this->tokens = tokens;
}

void Frizz::Parser::next_token() {
	if (!this->tokens.empty()) {
		this->cur_tok = *(this->tokens.begin());
	}
}

void Frizz::Parser::block() {
	if(this->optional_found(TokType::tok_block)) {
		// Do stuff
	}
}

void Frizz::Parser::preamble() {
	if(this->optional_found(TokType::tok_preamble)) {

	}
}

bool Frizz::Parser::optional_found(TokType id) {
	if (id == this->cur_tok.id) {
		this->next_token();
		return true;
	}

	return false;
}

bool Frizz::Parser::required_found(TokType id) {
	if (this->optional_found(id)) {
		return true;
	}

	std::string msg = "Expected " << id << ", Got: " << this->cur_tok.id;
	this->add_error(msg);
	return false;

}

bool Frizz::Parser::required_found(TokType id, std::string val) {
	if (id == this->cur_tok.id && this->cur_tok.value == val) {
		this->next_token();
		return true;
	}

	std::string msg = "Expected " << id << " with val " << val << ", Got: "
			<< this->cur_tok.id << " with val " << this->cur_tok.value;
	this->add_error(msg);
	return false;
}

void Frizz::Parser::add_error(std::string message) {
	this->errors.push_back(message);
}
