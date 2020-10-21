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
class ParseException : public std::runtime_error {
public:
  ParseException(std::string msg)
    : std::runtime_error(msg) {};
};

class Parser {
public:
  Parser(Frizz::FileUtility& util)
    : util(util) {};

  void next_token();
  void set_tokens(std::vector<Token> tokens);
  void parse();
  std::vector<std::reference_wrapper<const BasicAst>> get_trees();
  void clear_trees();

private:
  Frizz::FileUtility& util;
  Token cur_tok;
  int cur_line;
  std::string last_val;
  std::vector<Token> tokens;
  std::vector<std::string> errors;
  std::vector<std::unique_ptr<BasicAst>> trees;

private:
  void block();
  void ident();
  void passthrough();
  void context();
  void for_loop();
  void for_loop_children(ForLoopAst* const loop,
                         const std::vector<std::filesystem::path>& paths,
                         const int items_per_page,
                         const int num_pages);

  std::tuple<std::string, std::string> find_ident();

  bool peek_current(TokType id);
  bool optional_found(TokType id);
  bool optional_found(TokType id, std::string val);
  void required_found(TokType id);
  void required_found(TokType id, std::string val);
  void throw_error(std::string message);
  bool is_eof();
};
}
#endif /* SRC_PARSER_H_ */
