#ifndef _SRC_CONTEXT_PARSER_H_
#define _SRC_CONTEXT_PARSER_H_

#include <filesystem>
#include <unordered_map>
#include <vector>

#include "file_utility.h"
#include "lexer.h"
#include "maddy/parser.h"

namespace Frizz {

class ContextParser {
public:
  ContextParser(Frizz::FileUtility util)
    : util(util)
    , html_parser(maddy::Parser()) {};

  void set_tokens(std::vector<Frizz::Token> tokens);
  std::unordered_map<std::string, std::string> parse(std::string ctx_namespace,
                                                     std::filesystem::path file_path);

private:
  std::unordered_map<std::string, std::string> get_main_context(std::string ctx_namespace);
  std::string convert_to_html(std::string markdown);

private:
  Frizz::FileUtility util;
  std::vector<Frizz::Token> tokens;
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>> contexts;
  maddy::Parser html_parser;
};
}

#endif /* _SRC_CONTEXT_PARSER_H_ */