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

        if(!this->block()) {
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
        if(!this->ident() || this->peek_current(TokType::tok_preamble)) {
          break;
        }
      }
    }
    else if(this->peek_current(TokType::tok_ctx_name)) {
      this->context();
    }
    else {
      this->passthrough();
    }
  }
}

bool Frizz::Parser::context() {
  this->required_found(TokType::tok_ctx_name);
  std::string ctx_var_name = this->last_val;

  this->required_found(TokType::tok_ctx_val);
  std::string ctx_var_val = this->last_val;

  if(this->has_errors()) {
    return false;
  }

  std::shared_ptr<CtxReplacementAst> ast =
    std::make_shared<CtxReplacementAst>(ctx_var_name, ctx_var_val);

  this->trees.push_back(std::move(ast));

  return true;
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

bool Frizz::Parser::block() {
  if(this->peek_current(TokType::tok_ident)) {
    return this->ident();
  }
  else if(this->peek_current(TokType::tok_for)) {
    this->for_loop();
  }

  return false;
}

bool Frizz::Parser::for_loop() {
  this->required_found(TokType::tok_for);
  this->required_found(TokType::tok_ident);
  std::string ident_name = this->last_val;

  this->required_found(TokType::tok_in);
  this->required_found(TokType::tok_str);
  std::string ident_val = this->last_val;

  std::shared_ptr<ForLoopAst> loop = std::make_shared<ForLoopAst>(ident_name, ident_val);

  this->required_found(TokType::tok_src);
  this->required_found(TokType::tok_str);

  if(this->has_errors()) {
    return false;
  }

  std::string template_name = this->last_val;

  std::vector<std::filesystem::path> paths = this->util.get_partial_file_paths(ident_val);

  for(auto it = paths.begin(); it != paths.end(); ++it) {
    std::shared_ptr<AssignmentAst> assign =
      std::make_shared<AssignmentAst>("src", template_name);
    assign->set_parent(loop);

    assign->set_context_filepath(*it);
    this->trees.push_back(std::move(assign));
  }

  return true;
}

bool Frizz::Parser::ident() {
  this->required_found(TokType::tok_ident);
  std::string ident_name = this->last_val;

  this->required_found(TokType::tok_sym, "=");

  this->required_found(TokType::tok_str);
  std::string ident_val = this->last_val;

  if(this->has_errors())
    return false;

  std::shared_ptr<AssignmentAst> exp = std::make_shared<AssignmentAst>(ident_name, ident_val);

  this->trees.push_back(std::move(exp));
  return true;
}

bool Frizz::Parser::passthrough() {
  std::shared_ptr<BasicAst> exp = std::make_shared<PassthroughAst>(this->cur_tok.value);

  this->trees.push_back(std::move(exp));
  return true;
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

const std::vector<std::shared_ptr<Frizz::BasicAst>>& Frizz::Parser::get_trees() {
  return this->trees;
}

void Frizz::Parser::clear_trees() {
  this->trees.clear();
}