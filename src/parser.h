/*
 * parser.h
 *
 *  Created on: Aug 11, 2020
 *      Author: dmmettlach
 */

#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

#include <memory>

#include "abstract_syntax_trees.h"
#include "lexer.h"

namespace Frizz {
class Parser {
public:
  Parser(Frizz::FileUtility& util)
    : util(util) {};

  void next_token();
  void set_tokens(std::vector<Token> tokens);
  void parse();
  const std::vector<std::shared_ptr<BasicAst>>& get_structures();
  void clear_structures();

private:
  Frizz::FileUtility& util;
  Token cur_tok;
  std::string last_val;
  std::vector<Token> tokens;
  std::vector<std::string> errors;
  std::vector<std::shared_ptr<BasicAst>> structures;

private:
  bool block();
  bool ident();
  bool passthrough();
  bool context();

  bool peek_current(TokType id);
  bool optional_found(TokType id);
  bool optional_found(TokType id, std::string val);
  bool required_found(TokType id);
  bool required_found(TokType id, std::string val);
  void add_error(std::string message);
  bool has_errors();
};
}
#endif /* SRC_PARSER_H_ */
