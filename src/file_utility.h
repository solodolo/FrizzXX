/*
 * FILE_UTILITY.h
 *
 *  Created on: Aug 16, 2020
 *      Author: dmmettlach
 */

#ifndef FILE_UTILITY_H_
#define FILE_UTILITY_H_

#include <filesystem>
#include <stdexcept>
#include <vector>

#include "frizz_config.h"

namespace Frizz {

class InvalidFilePath : public std::runtime_error {
public:
  InvalidFilePath()
    : std::runtime_error("Tried to access an invalid path") {};
};

class FileUtility {
public:
  FileUtility(Frizz::FrizzConfig& config)
    : config(config) {};

  std::vector<std::filesystem::path> get_source_file_paths();
  std::vector<std::filesystem::path> get_content_source_paths();
  std::vector<std::filesystem::path> get_partial_file_paths(std::string subdir);
  std::vector<std::filesystem::path> get_content_file_paths(std::string subdir);

  std::filesystem::path get_partial_file_path(std::string filename);
  std::filesystem::path get_content_file_path(std::string filename);
  std::filesystem::path get_relative_source_path(std::filesystem::path path);
  std::string get_partial_contents(std::string filename);

private:
  bool is_valid_extension(std::string extension);
  bool is_valid_path(std::filesystem::path);

  std::vector<std::filesystem::path> get_file_paths(std::filesystem::path dir);

private:
  Frizz::FrizzConfig& config;
};
}

#endif /* FILE_UTILITY_H_ */