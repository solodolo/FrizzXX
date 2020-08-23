/*
 * FILE_UTILITY.h
 *
 *  Created on: Aug 16, 2020
 *      Author: dmmettlach
 */

#ifndef FILE_UTILITY_H_
#define FILE_UTILITY_H_

#include <filesystem>
#include <vector>

#include "frizz_config.h"

namespace Frizz {
class FileUtility {
public:
  FileUtility(Frizz::FrizzConfig& config)
    : config(config) {};

  std::vector<std::filesystem::path> get_source_file_paths();
  std::vector<std::filesystem::path> get_partial_file_paths(std::string subdir);
  std::string get_partial_contents(std::string filename);

private:
  bool is_valid_extension(std::string extension);

private:
  Frizz::FrizzConfig& config;
};
}

#endif /* FILE_UTILITY_H_ */