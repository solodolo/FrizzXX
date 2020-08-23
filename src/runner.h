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

#include "lexer.h"
#include "parser.h"
#include "file_utility.h"

namespace Frizz {
class Runner {
public:
  std::unordered_map<std::string, std::string> process_partial_preamble(
    std::filesystem::path file_path, Frizz::FileUtility& util);

  void process_source_file(Frizz::Lexer& lexer,
                           Frizz::Parser& parser,
                           Frizz::FileUtility& util,
                           std::filesystem::path input_path,
                           std::filesystem::path output_path);

  void process_source_files(Frizz::FrizzConfig& config);
};
}

#endif /* _SRC_RUNNER_H_ */