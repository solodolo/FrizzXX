/*
 * structures.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#include <filesystem>
#include <fstream>
#include <iostream>

#include "structures.h"

bool Frizz::AssignmentStruct::is_src() {
  return this->name == "src";
}

std::string Frizz::AssignmentStruct::load_from_file(const std::string& root_path) {
  std::string file_contents;

  std::filesystem::path input_path(root_path);
  input_path /= this->value;

  std::ifstream input(input_path);

  char c;
  while(input.get(c)) {
    file_contents.push_back(c);
  }

  input.close();
  return file_contents;
}

std::string Frizz::AssignmentStruct::do_evaluate(const std::string& root_path) {
  if(this->is_src()) {
    return this->load_from_file(root_path);
  }

  return this->value;
}

std::string Frizz::PassthroughStruct::do_evaluate(const std::string& root_path) {
  // Return the value unmodified
  return this->value;
}