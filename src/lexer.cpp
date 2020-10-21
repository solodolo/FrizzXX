/*
 * lexer.cpp
 *
 *  Created on: Aug 8, 2020
 *      Author: dmmettlach
 */
#include <fstream>
#include <iostream>

#include "lexer.h"

void Frizz::Lexer::lex(std::filesystem::path filepath) {
  std::ifstream input(filepath);

  if(input) {
    while(!input.eof()) {
      std::string line;
      std::getline(input, line);

      this->lex_line(line);
      this->add_token(Token(TokType::tok_ws, "\n"));
    }

    this->add_token(Token(TokType::tok_eof));
  }

  input.close();
}

std::vector<Frizz::Token> Frizz::Lexer::get_tokens() {
  return this->tokens;
}

void Frizz::Lexer::add_token(Token tok) {
  this->tokens.push_back(tok);
}

bool Frizz::Lexer::tok_is_a(TokType type) {
  return this->cur_tok.id == type;
}

void Frizz::Lexer::lex_line(std::string line) {
  std::smatch match;

  while(!line.empty()) {
    match = std::smatch();

    if(std::regex_search(line, match, whitespace)) {
      Token tok(tok_ws, match.str());
      this->add_token(tok);
    }
    else if(std::regex_search(line, match, block_pattern)) {
      Token tok(tok_block);
      this->add_token(tok);
    }
    else if(std::regex_search(line, match, preamble_pattern)) {
      Token tok(tok_preamble);
      this->add_token(tok);
    }
    else if(std::regex_search(line, match, for_pattern)) {
      Token tok(tok_for, "for");
      this->add_token(tok);
    }
    else if(std::regex_search(line, match, in_pattern)) {
      Token tok(tok_in, "in");
      this->add_token(tok);
    }
    else if(std::regex_search(line, match, paginate_pattern)) {
      this->paginate(match.str());
    }
    else if(std::regex_search(line, match, ctx_pattern)) {
      this->ctx(match.str());
    }
    else if(std::regex_search(line, match, ctx_str_pattern)) {
      this->ctx_str(match.str());
    }
    else if(std::regex_search(line, match, str_pattern)) {
      this->str(match.str());
    }
    else if(std::regex_search(line, match, ident_pattern)) {
      Token tok(tok_ident);

      std::string result_str = match.str();
      tok.value = result_str;

      this->add_token(tok);
    }
    else {
      // no match so discard the first character and try again
      Token tok(tok_sym);
      tok.value = line[0];
      this->add_token(tok);

      line = line.substr(1);
    }

    if(!match.empty()) {
      line.erase(match.position(0), match.length(0));
    }
  }
}

void Frizz::Lexer::print_tokens() {
  std::cout << "Current tokens: ";

  std::vector<Token> tokens = this->get_tokens();
  std::vector<Token>::iterator it = tokens.begin();
  for(; it != tokens.end(); ++it) {
    std::cout << it->to_string() << ", ";
  }

  std::cout << std::endl;
}

void Frizz::Lexer::clear_tokens() {
  this->tokens.clear();
}

void Frizz::Lexer::ctx(std::string results) {
  Token name(tok_ctx_name);
  Token val(tok_ctx_val);

  results = results.substr(1, results.length() - 2); // consume open and close curly braces

  std::size_t split_idx = results.find(".");

  name.value = results.substr(0, split_idx);
  val.value = results.substr(split_idx + 1, std::string::npos);

  this->add_token(name);
  this->add_token(val);
}

void Frizz::Lexer::ctx_str(std::string results) {
  Token open_qt(tok_sym, "\"");
  this->add_token(open_qt);

  results = results.substr(1, results.length() - 2); // consume start and end quotes
  this->ctx(results);

  Token close_qt(tok_sym, "\"");
  this->add_token(close_qt);
}

void Frizz::Lexer::str(std::string results) {
  Token tok(tok_str);

  // get the contents of the string witout the dbl quotes
  std::string val = results.substr(1, results.length() - 2);
  tok.value = val;
  this->add_token(tok);
}

void Frizz::Lexer::paginate(std::string results) {
  std::string per_page_str = results.substr(9); // number after `paginate=`

  Token tok(tok_paginate, per_page_str);
  this->add_token(tok);
}