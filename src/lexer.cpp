/*
 * lexer.cpp
 *
 *  Created on: Aug 8, 2020
 *      Author: dmmettlach
 */
#include <iostream>
#include <fstream>

#include "lexer.h"

void Frizz::Lexer::lex(std::filesystem::path filepath) {
  std::ifstream input(filepath);

  if(input) {
    while(!input.eof()) {
      std::getline(input, this->line);
      this->next_tok();
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

void Frizz::Lexer::next_tok() {
  std::string cur_line = this->line;
  std::smatch results;

  while(!cur_line.empty()) {
    results = std::smatch();

    if(std::regex_search(cur_line, results, whitespace)) {
      Token tok(tok_ws, results.str());
      this->add_token(tok);
    }
    else if(std::regex_search(cur_line, results, block_pattern)) {
      Token tok(tok_block);
      this->add_token(tok);
    }
    else if(std::regex_search(cur_line, results, preamble_pattern)) {
      Token tok(tok_preamble);
      this->add_token(tok);
    }
    else if(std::regex_search(cur_line, results, for_pattern)) {
      Token tok(tok_for, "for");
      this->add_token(tok);
    }
    else if(std::regex_search(cur_line, results, in_pattern)) {
      Token tok(tok_in, "in");
      this->add_token(tok);
    }
    else if(std::regex_search(cur_line, results, ctx_pattern)) {
      Token name(tok_ctx_name);
      Token val(tok_ctx_val);

      std::string result_str = results.str();
      result_str =
        result_str.substr(1, result_str.length() - 2); // consume open and close curly braces

      std::size_t split_idx = result_str.find(".");

      name.value = result_str.substr(0, split_idx);
      val.value = result_str.substr(split_idx + 1, std::string::npos);

      this->add_token(name);
      this->add_token(val);
    }
    else if(std::regex_search(cur_line, results, str_pattern)) {
      Token tok(tok_str);

      // get the contents of the string witout the dbl quotes
      std::string result_str = results.str();
      tok.value = result_str.substr(1, result_str.length() - 2);

      this->add_token(tok);
    }
    else if(std::regex_search(cur_line, results, ident_pattern)) {
      Token tok(tok_ident);

      std::string result_str = results.str();
      tok.value = result_str;

      this->add_token(tok);
    }
    else {
      // no match so discard the first character and try again
      Token tok(tok_sym);
      tok.value = cur_line[0];
      this->add_token(tok);

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