/*
 * frizz_config.cpp
 *
 *  Created on: Aug 13, 2020
 *      Author: dmmettlach
 */

#include <fstream>

#include "frizz_config.h"
#include "nlohmann/json.hpp"

bool Frizz::FrizzConfig::load_configuration(std::string path_to_config) {
  std::ifstream input(path_to_config);
  nlohmann::json json_config;

  input >> json_config;

  if(json_config.contains(Frizz::FrizzConfig::PARENT_DIR)) {
    std::string dir = json_config[Frizz::FrizzConfig::PARENT_DIR];
    this->set_parent_dir(dir);
  }
  else {
    return false;
  }

  return true;
}

void Frizz::FrizzConfig::set_parent_dir(std::string path) {
  this->parent_dir = std::filesystem::canonical(path);
}

std::filesystem::path Frizz::FrizzConfig::get_source_root_path() {
  return this->parent_dir / this->source_dir;
}

std::filesystem::path Frizz::FrizzConfig::get_partial_templates_path() {
  return this->parent_dir / this->partial_dir;
}

std::filesystem::path Frizz::FrizzConfig::get_build_path() {
  return this->parent_dir / this->build_dir;
}

std::filesystem::path Frizz::FrizzConfig::get_content_path() {
  return this->parent_dir / this->content_dir;
}