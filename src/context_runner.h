#ifndef _SRC_CONTEXT_RUNNER_H_
#define _SRC_CONTEXT_RUNNER_H_

#include <filesystem>
#include <unordered_map>
#include <vector>

#include "file_utility.h"
#include "lexer.h"
#include "maddy/parser.h"

namespace Frizz {

class ContextRunner {
public:
  ContextRunner()
    : html_parser(maddy::Parser()) {};

  std::unordered_map<std::string, std::string> process(const std::string context_namespace,
                                                       const std::filesystem::path file_path,
                                                       Frizz::FileUtility& util);
  void set_tokens(std::vector<Frizz::Token> tokens);

private:
  std::unordered_map<std::string, std::string> process_file(std::string ctx_namespace,
                                                            std::filesystem::path file_path);
  std::unordered_map<std::string, std::string> get_main_context(std::string ctx_namespace,
                                                                std::vector<Frizz::Token> tokens,
                                                                Frizz::FileUtility& util);
  std::string convert_to_html(std::string markdown);
  std::string get_namespaced_key(std::string name_space, std::string key);

private:
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>> contexts;
  maddy::Parser html_parser;
};
}

#endif /* _SRC_CONTEXT_RUNNER_H_ */