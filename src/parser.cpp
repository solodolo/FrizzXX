/*
 * parser.cpp
 *
 *  Created on: Aug 11, 2020
 *      Author: dmmettlach
 */
#include <cmath>
#include <iostream>

#include "parser.h"

void Frizz::Parser::parse() {
  this->cur_line = 1;
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

      this->optional_found(TokType::tok_ws, "\n");
    }
    else if(this->peek_current(TokType::tok_preamble)) {
      this->required_found(TokType::tok_preamble);
      this->required_found(TokType::tok_ws);

      while(true) {
        this->ident();
        this->required_found(TokType::tok_ws);

        if(this->peek_current(TokType::tok_preamble)) {
          this->required_found(TokType::tok_preamble);
          this->optional_found(TokType::tok_ws, "\n");
          break;
        }
      }

      this->optional_found(TokType::tok_ws, "\n");
    }
    else if(this->peek_current(TokType::tok_ctx_name)) {
      // using a context variable
      // i.e. **{post.title}**
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

  std::unique_ptr<CtxReplacementAst> ast =
    std::make_unique<CtxReplacementAst>(ctx_var_name, ctx_var_val);

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
  if(this->cur_tok.id == TokType::tok_ws && this->cur_tok.value == "\n") {
    ++this->cur_line;
  }

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

  std::vector<std::filesystem::path> paths = this->util.get_content_file_paths(ident_val);
  std::sort(paths.begin(), paths.end());

  std::unique_ptr<ForLoopAst> loop = std::make_unique<ForLoopAst>(ident_name, ident_val);

  float num_items = paths.size();
  int items_per_page = num_items;
  int num_pages = 1;

  if(this->optional_found(TokType::tok_paginate)) {
    std::string per_page_str = this->last_val;
    items_per_page = std::stoi(per_page_str);
    num_pages = std::ceil(num_items / items_per_page);
  }

  this->for_loop_children(loop.get(), paths, items_per_page, num_pages);

  this->trees.push_back(std::move(loop));
}

void Frizz::Parser::for_loop_children(Frizz::ForLoopAst* const loop,
                                      const std::vector<std::filesystem::path>& paths,
                                      const int items_per_page,
                                      const int num_pages) {

  this->required_found(TokType::tok_ws, "\n");
  this->required_found(TokType::tok_block);
  this->required_found(TokType::tok_ws);

  std::tuple<std::string, std::string> assign = this->find_ident();

  std::string template_key = std::get<0>(assign);
  std::string template_name = std::get<1>(assign);

  bool paginate = num_pages > 1;
  int cur_page = 1;
  for(std::size_t i = 0; i < paths.size(); ++i) {
    if(paginate && (i % items_per_page == 0)) {
      std::unique_ptr<PageAst> page =
        std::make_unique<PageAst>(cur_page, num_pages, items_per_page);
      
      // add children to page
      // insert page ast into trees

      ++cur_page;
    }
    std::unique_ptr<AssignmentAst> assign =
      std::make_unique<AssignmentAst>(template_key, template_name);

    if(assign->is_src()) {
      this->throw_error("Cannot use a src assignment in a for loop context. Try 'use' instead.");
    }

    assign->set_namespace(loop->get_key());
    assign->set_context_filepath(paths.at(i));

    loop->add_child(std::move(assign));
  }
}

void Frizz::Parser::ident() {
  std::tuple<std::string, std::string> ident = this->find_ident();

  std::unique_ptr<AssignmentAst> exp =
    std::make_unique<AssignmentAst>(std::get<0>(ident), std::get<1>(ident));

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
  std::unique_ptr<BasicAst> exp = std::make_unique<PassthroughAst>(this->cur_tok.value);

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
    std::string msg =
      "Expected: " + Frizz::TokNames[id] + ", Got: " + Frizz::TokNames[this->cur_tok.id];
    this->throw_error(msg);
  }
}

void Frizz::Parser::required_found(TokType id, std::string val) {
  if(!this->optional_found(id, val)) {
    std::string msg = "Expected " + Frizz::TokNames[id] + " with val " + val +
                      ", Got: " + Frizz::TokNames[this->cur_tok.id] + " with val " +
                      this->cur_tok.value;
    this->throw_error(msg);
  }
}

void Frizz::Parser::throw_error(std::string message) {
  message = "Parse error on line " + std::to_string(this->cur_line) + ". " + message;
  throw Frizz::ParseException(message);
}

std::vector<std::reference_wrapper<const Frizz::BasicAst>> Frizz::Parser::get_trees() {
  std::vector<std::reference_wrapper<const Frizz::BasicAst>> references;
  references.reserve(this->trees.size());

  for(auto it = this->trees.begin(); it != this->trees.end(); ++it) {
    references.push_back(std::cref(*it->get()));
  }

  return references;
}

void Frizz::Parser::clear_trees() {
  this->trees.clear();
}

bool Frizz::Parser::is_eof() {
  return this->cur_tok.id == TokType::tok_eof;
}