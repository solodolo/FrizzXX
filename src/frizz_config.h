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
  FrizzConfig()
    : source_dir("sources")
    , partial_dir("partials")
    , build_dir("output")
    , content_dir("content") {};

  bool load_configuration(std::string path_to_config);
  void set_parent_dir(std::string path);
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