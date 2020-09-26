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
  std::unordered_map<std::string, std::string> process_partial_preamble(
    std::string context_namespace, std::filesystem::path file_path, Frizz::FileUtility& util);

  void process_source_file(Frizz::Lexer& lexer,
                           Frizz::Parser& parser,
                           Frizz::FileUtility& util,
                           std::filesystem::path input_path,
                           std::filesystem::path output_path);

  void process_source_files(Frizz::FrizzConfig& config);

  void process_content_source_files(Frizz::FrizzConfig& config);

  std::string process_with_context(std::filesystem::path file_path,
                                   std::unordered_map<std::string, std::string> context,
                                   Frizz::FileUtility& util);

  void convert_to_html(Frizz::FrizzConfig& config);
};
}

#endif /* _SRC_RUNNER_H_ */