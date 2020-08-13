/*
 * parser.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: dmmettlach
 */

#include "parser.h"
#include "expressions.h"

void Frizz::Parser::parse() {
	while(!this->tokens.empty()) {
		this->next_token();

		this->preamble();
		this->block();
	}
}

void Frizz::Parser::set_tokens(std::vector<Token> tokens) {
	this->tokens = tokens;
}

void Frizz::Parser::next_token() {
	if (!this->tokens.empty()) {
		this->cur_tok = *(this->tokens.begin());

		this->tokens.erase(this->tokens.begin());
	}
}

void Frizz::Parser::block() {
	if(this->optional_found(TokType::tok_block)) {

	}
}

void Frizz::Parser::preamble() {
	if(this->optional_found(TokType::tok_preamble)) {

	}
}

bool Frizz::Parser::optional_found(TokType id) {
	if (id == this->cur_tok.id) {
		this->last_val = this->cur_tok.value;
		this->next_token();
		return true;
	}

	return false;
}

bool Frizz::Parser::required_found(TokType id) {
	if (this->optional_found(id)) {
		return true;
	}

	std::string msg = "Expected " + std::to_string(id) + ", Got: " + std::to_string(this->cur_tok.id);
	this->add_error(msg);
	return false;

}

bool Frizz::Parser::required_found(TokType id, std::string val) {
	if (id == this->cur_tok.id && this->cur_tok.value == val) {
		this->next_token();
		return true;
	}

	std::string msg = "Expected " + std::to_string(id) + " with val " + val + ", Got: "
			+ std::to_string(this->cur_tok.id) + " with val " + this->cur_tok.value;
	this->add_error(msg);
	return false;
}

void Frizz::Parser::add_error(std::string message) {
	this->errors.push_back(message);
}

bool Frizz::Parser::has_errors() {
	return !this->errors.empty();
}
