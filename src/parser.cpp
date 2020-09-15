/*
 * parser.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: dmmettlach
 */
#include <iostream>

#include "parser.h"

void Frizz::Parser::parse() {
  this->next_token();
  while(!this->is_eof()) {
    if(this->peek_current(TokType::tok_block)) {
      while(true) {
        this->required_found(TokType::tok_block);
        this->required_found(TokType::tok_ws);

        this->block();

        if(!this->optional_found(Frizz::TokType::tok_sym, ",")) {
          break;
        }
      }

      this->optional_found(TokType::tok_nl);
    }
    else if(this->peek_current(TokType::tok_preamble)) {
      this->required_found(TokType::tok_preamble);

      while(true) {
        this->ident();

        if(this->peek_current(TokType::tok_preamble)) {
          break;
        }
      }

      this->optional_found(TokType::tok_nl);
    }
    else if(this->peek_current(TokType::tok_ctx_name)) {
      this->context();
    }
    else {
      this->passthrough();
    }
  }
}

void Frizz::Parser::context() {
  this->required_found(TokType::tok_ctx_name);
  std::string ctx_var_name = this->last_val;

  this->required_found(TokType::tok_ctx_val);
  std::string ctx_var_val = this->last_val;

  std::shared_ptr<CtxReplacementAst> ast =
    std::make_shared<CtxReplacementAst>(ctx_var_name, ctx_var_val);

  this->trees.push_back(std::move(ast));
}

void Frizz::Parser::set_tokens(std::vector<Token> tokens) {
  this->tokens = tokens;

  if(this->tokens.back().id != TokType::tok_eof) {
    Token eof(TokType::tok_eof);
    this->tokens.push_back(eof);
  }
}

void Frizz::Parser::next_token() {
  this->last_val = this->cur_tok.value;

  if(!this->tokens.empty()) {
    this->cur_tok = *(this->tokens.begin());

    this->tokens.erase(this->tokens.begin());
  }
}

void Frizz::Parser::block() {
  if(this->peek_current(TokType::tok_ident)) {
    this->ident();
  }
  else if(this->peek_current(TokType::tok_for)) {
    this->for_loop();
  }
}

void Frizz::Parser::for_loop() {
  this->required_found(TokType::tok_for);
  this->required_found(TokType::tok_ws);
  this->required_found(TokType::tok_ident);

  std::string ident_name = this->last_val;

  this->required_found(TokType::tok_ws);
  this->required_found(TokType::tok_in);
  this->required_found(TokType::tok_ws);
  this->required_found(TokType::tok_str);
  std::string ident_val = this->last_val;

  std::shared_ptr<ForLoopAst> loop = std::make_shared<ForLoopAst>(ident_name, ident_val);

  this->required_found(TokType::tok_nl);
  this->required_found(TokType::tok_block);
  this->required_found(TokType::tok_ws);

  std::tuple<std::string, std::string> assign = this->find_ident();

  std::string template_key = std::get<0>(assign);
  std::string template_name = std::get<1>(assign);

  std::vector<std::filesystem::path> paths = this->util.get_partial_file_paths(ident_val);

  std::sort(paths.begin(), paths.end());

  for(auto it = paths.begin(); it != paths.end(); ++it) {
    std::shared_ptr<AssignmentAst> assign =
      std::make_shared<AssignmentAst>(template_key, template_name);
    // TODO: Parent is set only to retreive namespace later. Instead of copying entire parent here,
    // just set namespace directly
    assign->set_parent(loop);

    assign->set_context_filepath(*it);

    loop->add_child(assign);
  }

  this->trees.push_back(std::move(loop));
}

void Frizz::Parser::ident() {
  std::tuple<std::string, std::string> ident = this->find_ident();

  std::shared_ptr<AssignmentAst> exp =
    std::make_shared<AssignmentAst>(std::get<0>(ident), std::get<1>(ident));

  this->trees.push_back(std::move(exp));
}

std::tuple<std::string, std::string> Frizz::Parser::find_ident() {
  this->required_found(TokType::tok_ident);
  std::string ident_name = this->last_val;

  this->required_found(TokType::tok_sym, "=");

  this->required_found(TokType::tok_str);
  std::string ident_val = this->last_val;

  return std::make_tuple(ident_name, ident_val);
}

void Frizz::Parser::passthrough() {
  std::shared_ptr<BasicAst> exp = std::make_shared<PassthroughAst>(this->cur_tok.value);

  this->trees.push_back(std::move(exp));

  this->next_token();
}

bool Frizz::Parser::peek_current(TokType id) {
  return id == this->cur_tok.id;
}

bool Frizz::Parser::optional_found(TokType id) {
  if(id == this->cur_tok.id) {
    this->next_token();
    return true;
  }

  return false;
}

bool Frizz::Parser::optional_found(TokType id, std::string val) {
  if(id == this->cur_tok.id && val == this->cur_tok.value) {
    this->next_token();
    return true;
  }

  return false;
}

void Frizz::Parser::required_found(TokType id) {
  if(!this->optional_found(id)) {
    std::string msg = "Expected " + std::to_string(id) + ", Got: " + std::to_string(this->cur_tok.id);
    this->throw_error(msg);
  }
}

void Frizz::Parser::required_found(TokType id, std::string val) {
  if(!this->optional_found(id, val)) {
    std::string msg = "Expected " + std::to_string(id) + " with val " + val +
                    ", Got: " + std::to_string(this->cur_tok.id) + " with val " +
                    this->cur_tok.value;
    this->throw_error(msg);
  }
}

void Frizz::Parser::throw_error(std::string message) {
  throw Frizz::ParseException(message);
}

const std::vector<std::shared_ptr<Frizz::BasicAst>>& Frizz::Parser::get_trees() {
  return this->trees;
}

void Frizz::Parser::clear_trees() {
  this->trees.clear();
}

bool Frizz::Parser::is_eof() {
  return this->cur_tok.id == TokType::tok_eof;
}