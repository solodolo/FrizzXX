/*
 * frizz_config.h
 *
 *  Created on: Aug 13, 2020
 *      Author: dmmettlach
 */

#ifndef FRIZZ_CONFIG_H_
#define FRIZZ_CONFIG_H_

#include <filesystem>
#include <string>

namespace Frizz {
class FrizzConfig {
public:
  void load_configuration(std::string path_to_config);
  std::string get_source_root_path();
  std::string get_build_path();
  std::filesystem::path get_path_to_input_file(std::string filename);
  std::filesystem::path get_path_to_output_file(std::string filename);

private:
  inline const static std::string BASE_TEMPLATES_PATH_KEY = "base_templates_path";
  inline const static std::string PARTIAL_TEMPLATES_PATH_KEY = "partial_templates_path";
  inline const static std::string BUILD_PATH_KEY = "build_path";
  
  std::string source_root_path;
  std::string build_path;
};
}

#endif