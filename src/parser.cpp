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
	this->cur_tok = *this->tokens.begin();

	this->block();
}

void Frizz::Parser::block() {

}

bool Frizz::Parser::can_have(TokType id) {
	if(id == this->cur_tok.id) {
		this->next_token();
		return true;
	}

	return false;
}

bool Frizz::Parser::must_have(TokType id) {
	if(this->can_have(id)) {
		return true;
	}

	std::string msg = "Expected " << id << ", Got: " << this->cur_tok.id;
	this->add_error(msg);
	return false;

}

bool Frizz::Parser::must_have(TokType id, std::string val) {
	if(id == this->cur_tok.id && this->cur_tok.value == val) {
		this->next_token();
		return true;
	}

	std::string msg = "Expected " << id << " with val " << val << ", Got: " << this->cur_tok.id << " with val " << this->cur_tok.value;
	this->add_error(msg);
	return false;
}

void Frizz::Parser::add_error(std::string message) {
	this->errors.push_back(message);
}
