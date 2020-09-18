/*
 * file_utility.cpp
 *
 *  Created on: Aug 16, 2020
 *      Author: dmmettlach
 */

#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "file_utility.h"

bool Frizz::FileUtility::is_valid_extension(std::string extension) {
  // just md for now
  return extension == ".md" || extension == ".html";
}

/*
TODO: Check the root of path up to parent dir length to see if it equals parent dir
*/
bool Frizz::FileUtility::is_valid_path(std::filesystem::path path) {
  for(const auto& piece : path) { 
    if(piece == "..") {
      return false;
    }
  }

  return true;
}

std::vector<std::filesystem::path> Frizz::FileUtility::get_source_file_paths() {
  std::vector<std::filesystem::path> source_paths;

  std::filesystem::path source_root = this->config.get_source_root_path();
  std::filesystem::directory_iterator source_it(source_root);

  for(auto& entry : source_it) {
    std::filesystem::path path = entry.path();
    if(!this->is_valid_extension(path.extension())) {
      continue;
    }

    source_paths.push_back(path);
  }

  return source_paths;
}

/*
  gets the part of path relative to the source path 
  allows the directory structure can be maintained in the output
*/
std::filesystem::path Frizz::FileUtility::get_relative_source_path(std::filesystem::path path) {
  std::filesystem::path base = this->config.get_source_root_path();
  std::filesystem::path rel_path = std::filesystem::relative(path, base);

  if(this->is_valid_path(rel_path) && rel_path != ".") {
    return rel_path;
  }

  throw Frizz::InvalidFilePath();
}

std::vector<std::filesystem::path> Frizz::FileUtility::get_file_paths(std::filesystem::path dir) {
  std::vector<std::filesystem::path> paths;
  
  if(this->is_valid_path(dir)) {
    if(std::filesystem::exists(dir)) {
      std::filesystem::directory_iterator it(dir);
      
      for(auto f : it) {
        paths.push_back(f);
      }
    }

    return paths;
  }

  throw Frizz::InvalidFilePath();
}

std::vector<std::filesystem::path> Frizz::FileUtility::get_content_file_paths(std::string subdir) {
  std::filesystem::path dir = this->config.get_content_path() / subdir;
  return this->get_file_paths(dir);
}

std::filesystem::path Frizz::FileUtility::get_partial_file_path(std::string filename) {
  std::filesystem::path p = this->config.get_partial_templates_path() / filename;

  if(this->is_valid_path(p)) {
    return p;
  }
  
  throw Frizz::InvalidFilePath();
}

std::filesystem::path Frizz::FileUtility::get_content_file_path(std::string filename) {
  std::filesystem::path p = this->config.get_content_path() / filename;

  if(this->is_valid_path(p)) {
    return p;
  }
  
  throw Frizz::InvalidFilePath();
}

std::string Frizz::FileUtility::get_partial_contents(std::string filename) {
  std::string contents;
  std::filesystem::path path = this->config.get_partial_templates_path() / filename;

  if(this->is_valid_path(path) && this->is_valid_extension(path.extension())) {

    std::ifstream input(path);
    char c;
    while(input.get(c)) {
      contents.push_back(c);
    }

    input.close();

    return contents;
  }

  throw Frizz::InvalidFilePath();
}
