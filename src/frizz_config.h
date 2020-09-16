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
  bool load_configuration(std::string path_to_config);
  std::filesystem::path get_source_root_path();
  std::filesystem::path get_partial_templates_path();
  std::filesystem::path get_build_path();
  std::filesystem::path get_content_path();

private:
  inline const static std::string PARENT_DIR = "parent_dir";
  
  std::filesystem::path parent_dir;
  std::filesystem::path source_dir;
  std::filesystem::path partial_dir;
  std::filesystem::path build_dir;
  std::filesystem::path content_dir;
};
}

#endif