/*
 * parser.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: dmmettlach
 */
#include <iostream>

#include "parser.h"

void Frizz::Parser::parse() {
  while(!this->tokens.empty()) {
    this->next_token();
    if(this->peek_current(TokType::tok_block)) {
      while(true) {
        this->required_found(TokType::tok_block);

        if(std::unique_ptr<BasicAst> exp = this->block()) {
          this->structures.push_back(std::move(exp));
        }
        else {
          break;
        }

        if(this->has_errors() || !this->optional_found(Frizz::TokType::tok_sym, ",")) {
          break;
        }
      }
    }
    else if(this->peek_current(TokType::tok_preamble)) {
      this->required_found(TokType::tok_preamble);

      while(true) {
        if(std::unique_ptr<BasicAst> exp = this->ident()) {
          this->structures.push_back(std::move(exp));
        }
        else {
          break;
        }

        if(this->peek_current(TokType::tok_preamble)) {
          break;
        }
      }
    }
    else {
      std::unique_ptr<BasicAst> exp = this->passthrough();
      this->structures.push_back(std::move(exp));
    }
  }
}

void Frizz::Parser::set_tokens(std::vector<Token> tokens) {
  this->tokens = tokens;
}

void Frizz::Parser::next_token() {
  if(!this->tokens.empty()) {
    this->cur_tok = *(this->tokens.begin());

    this->tokens.erase(this->tokens.begin());
  }
}

std::unique_ptr<Frizz::BasicAst> Frizz::Parser::block() {
  if(this->peek_current(TokType::tok_ident)) {
    return std::move(this->ident());
  }
  else if(this->peek_current(TokType::tok_for)) {
    this->required_found(TokType::tok_for);
    this->required_found(TokType::tok_ident);
    std::string ident_name = this->last_val;

    this->required_found(TokType::tok_in);
    this->required_found(TokType::tok_str);
    std::string ident_val = this->last_val;

    std::unique_ptr<BasicAst> exp = std::make_unique<ForLoopAst>(ident_name, ident_val);
  }

  return nullptr;
}

std::unique_ptr<Frizz::BasicAst> Frizz::Parser::ident() {
  this->required_found(TokType::tok_ident);
  std::string ident_name = this->last_val;

  this->required_found(TokType::tok_sym, "=");

  this->required_found(TokType::tok_str);
  std::string ident_val = this->last_val;

  std::unique_ptr<AssignmentAst> exp = std::make_unique<AssignmentAst>(ident_name, ident_val);

  if(this->has_errors())
    return nullptr;
  return std::move(exp);
}

std::unique_ptr<Frizz::BasicAst> Frizz::Parser::passthrough() {
  std::unique_ptr<BasicAst> exp = std::make_unique<PassthroughAst>(this->cur_tok.value);

  return std::move(exp);
}

bool Frizz::Parser::peek_current(TokType id) {
  return id == this->cur_tok.id;
}

bool Frizz::Parser::optional_found(TokType id) {
  if(id == this->cur_tok.id) {
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
  if(this->optional_found(id)) {
    return true;
  }

  std::string msg = "Expected " + std::to_string(id) + ", Got: " + std::to_string(this->cur_tok.id);
  this->add_error(msg);
  return false;
}

bool Frizz::Parser::required_found(TokType id, std::string val) {
  if(this->optional_found(id, val)) {
    return true;
  }

  std::string msg = "Expected " + std::to_string(id) + " with val " + val +
                    ", Got: " + std::to_string(this->cur_tok.id) + " with val " +
                    this->cur_tok.value;
  this->add_error(msg);
  return false;
}

void Frizz::Parser::add_error(std::string message) {
  this->errors.push_back(message);
}

bool Frizz::Parser::has_errors() {
  return !this->errors.empty();
}

const std::vector<std::unique_ptr<Frizz::BasicAst>>& Frizz::Parser::get_structures() {
  return this->structures;
}

void Frizz::Parser::clear_structures() {
  this->structures.clear();
}