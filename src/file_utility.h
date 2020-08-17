/*
 * FILE_UTILITY.h
 *
 *  Created on: Aug 16, 2020
 *      Author: dmmettlach
 */

#ifndef FILE_UTILITY_H_
#define FILE_UTILITY_H_

#include <filesystem>

#include "lexer.h"
#include "parser.h"

namespace Frizz {
class FileUtility {
public:
  FileUtility(Frizz::Lexer& lexer, Frizz::Parser& parser)
    : lexer(lexer)
    , parser(parser) {};

  void process_source_files(std::filesystem::path source_files_path,
                            std::filesystem::path replacement_lookup_path,
                            std::filesystem::path build_path);

private:
  void process_source_file(std::filesystem::path file_path,
                           std::filesystem::path replacement_lookup_path,
                           std::filesystem::path build_path);
  Frizz::Lexer& lexer;
  Frizz::Parser& parser;
};
}

#endif /* FILE_UTILITY_H_ */