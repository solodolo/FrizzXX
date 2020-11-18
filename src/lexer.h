/*
 * lexer.h
 *
 *  Created on: Aug 8, 2020
 *      Author: dmmettlach
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <filesystem>
#include <memory>
#include <regex>
#include <string>
#include <vector>

namespace Frizz {
enum TokType {
  tok_none,
  tok_block,
  tok_preamble,
  tok_ident,
  tok_str,
  tok_for,
  tok_in,
  tok_do,
  tok_done,
  tok_paginate,
  tok_paginator,
  tok_sym,
  tok_ctx_name,
  tok_ctx_val,
  tok_nl,
  tok_ws,
  tok_eof
};

const std::string TokNames[] = { "None",         "Block",         "Preamble",  "Identifier",
                                 "String",       "For",           "In",        "Do",
                                 "Done",         "Paginate",      "Paginator", "Symbol",
                                 "Context Name", "Context Value", "New Line",  "Whitespace",
                                 "End of File" };

struct Token {
  TokType id;

  Token()
    : id(TokType::tok_none) {};

  Token(TokType id)
    : id(id) {};

  Token(TokType id, std::string value)
    : id(id)
    , value(value) {};

  std::string value;
  std::string to_string() { return std::to_string(id); }
};

class Lexer {
public:
  Token cur_tok;

public:
  Lexer()
    : cur_tok(tok_none)
    , whitespace("^\\s+")
    , block_pattern("^@@")
    , preamble_pattern("^~~$")
    , str_pattern("^\"[^\n]+?\"")
    , ident_pattern("^[a-zA-Z_-]+")
    , for_pattern("^for")
    , in_pattern("^in")
    , do_pattern("^do")
    , done_pattern("^done")
    , paginate_pattern("^paginate=[0-9]+")
    , paginator_pattern("^\\{_paginator\\.[a-zA-Z]+}")
    , ctx_pattern("^\\{[a-zA-Z]*\\.[a-zA-Z]+}")
    , ctx_str_pattern("^\"\\{[a-zA-Z]*\\.[a-zA-Z]+}\"") {}

  void lex(std::filesystem::path path);
  void lex_line(std::string line);
  bool tok_is_a(TokType type);
  std::vector<Token> get_tokens();
  void add_token(Token tok);
  void clear_tokens();

private:
  std::vector<Token> tokens;

  std::regex whitespace;
  std::regex block_pattern;
  std::regex preamble_pattern;
  std::regex str_pattern;
  std::regex ident_pattern;
  std::regex for_pattern;
  std::regex in_pattern;
  std::regex do_pattern;
  std::regex done_pattern;
  std::regex paginate_pattern;
  std::regex paginator_pattern;
  std::regex ctx_pattern;
  std::regex ctx_str_pattern;

  void str(std::string results);
  void ctx_str(std::string results);
  void ctx(std::string results);
  void paginate(std::string results);
  void paginator(std::string results);

private:
  void print_tokens();
};
}

#endif /* LEXER_H_ */
