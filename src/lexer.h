/*
 * lexer.h
 *
 *  Created on: Aug 8, 2020
 *      Author: dmmettlach
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <filesystem>
#include <string>
#include <vector>

namespace Frizz {
enum TokType {
  tok_none = -1,
  tok_block = 1,
  tok_preamble = 2,
  tok_ident = 3,
  tok_str = 4,
  tok_for = 6,
  tok_in = 7,
  tok_sym = 8,
  tok_ctx_name = 9,
  tok_ctx_val = 10,
  tok_nl = 11,
  tok_eof = 12
};

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
    : cur_tok(tok_none) {};

  void lex(std::filesystem::path path);
  void next_tok();
  void set_line(std::string line);
  bool tok_is_a(TokType type);
  std::vector<Token> get_tokens();
  void add_token(Token tok);
  void clear_tokens();

private:
  std::string line;
  std::vector<Token> tokens;

private:
  void print_tokens();
};
}

#endif /* LEXER_H_ */
