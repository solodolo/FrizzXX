/*
 * parser.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: dmmettlach
 */

#include "parser.h"

void Frizz::Parser::parse() {
    while(!this->tokens.empty()) {
        this->next_token();

        if(this->peek_current(TokType::tok_block)) {
            while(true) {
                this->required_found(TokType::tok_block);

                if(std::unique_ptr<BasicStruct> exp = this->block()) {
                    this->structures.push_back(std::move(exp));
                }
                else {
                    break;
                }
                
                if(this->cur_tok.value != ",") {
                    break;
                }
            }
        }
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

std::unique_ptr<Frizz::BasicStruct> Frizz::Parser::block() {
    if(this->peek_current(TokType::tok_ident)) {
        this->required_found(TokType::tok_ident);
        std::string ident_name = this->last_val;

        this->required_found(TokType::tok_sym, "=");

        this->required_found(TokType::tok_str);
        std::string ident_val = this->last_val;

        std::unique_ptr<BasicStruct> exp = std::make_unique<AssignmentStruct>(ident_name, ident_val);

        return std::move(exp);
    }

    return nullptr;
}

void Frizz::Parser::preamble() {
    if(this->optional_found(TokType::tok_preamble)) {

    }
}

bool Frizz::Parser::peek_current(TokType id) {
    return id == this->cur_tok.id;
}

bool Frizz::Parser::optional_found(TokType id) {
    if (id == this->cur_tok.id) {
        this->last_val = this->cur_tok.value;
        this->next_token();
        return true;
    }

    return false;
}

bool Frizz::Parser::optional_found(TokType id, std::string val) {
    if(id == this->cur_tok.id && val == this->cur_tok.value) {
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
    if (this->optional_found(id, val)) {
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
