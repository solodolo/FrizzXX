/*
 * runner.h
 *
 *  Created on: Aug 22, 2020
 *      Author: dmmettlach
 */

#ifndef _SRC_RUNNER_H_
#define _SRC_RUNNER_H_

#include <filesystem>
#include <unordered_map>

#include "file_utility.h"
#include "lexer.h"
#include "parser.h"

namespace Frizz {
class Runner {
public:
  void process_source_files(Frizz::FrizzConfig& config);
  void process_content_source_files(Frizz::FrizzConfig& config);
  
  std::unordered_map<std::string, std::string> process_file_preamble(
    std::string context_namespace, std::filesystem::path file_path, Frizz::FileUtility& util);

  void process_source_file(Frizz::Lexer& lexer,
                           Frizz::Parser& parser,
                           Frizz::FileUtility& util,
                           std::filesystem::path input_path,
                           std::filesystem::path output_path);

  std::string process_with_context(std::filesystem::path file_path,
                                   std::unordered_map<std::string, std::string> context,
                                   Frizz::FileUtility& util);

private:
  std::string process_ast_children(std::vector<std::shared_ptr<Frizz::BasicAst>> children,
                                   Frizz::FileUtility& util);
};
}

#endif /* _SRC_RUNNER_H_ */