//============================================================================
// Name        : frizz++.cpp
// Author      : Drew Mettlach
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "file_utility.h"
#include "frizz_config.h"
#include "lexer.h"
#include "maddy/parser.h"
#include "parser.h"

int main(int argc, char** argv) {
  int ret_val = 0;

  if(argc > 1) {
    std::string config_file_path = std::string(argv[1]);

    Frizz::FrizzConfig config;
    if(!config.load_configuration(config_file_path)) {
      std::cout << "Invalid config file" << std::endl;
      ret_val = -1;
    }
    else {
      Frizz::Lexer lexer;
      Frizz::Parser parser;

      Frizz::FileUtility util(lexer, parser);
      util.process_source_files(config.get_source_root_path(),
                                config.get_partial_templates_path(),
                                config.get_build_path());
    }
  }
  else {
    std::cout << "usage: FrizzXX /path/to/config.json" << std::endl;
    ret_val = -1;
  }

  return ret_val;
}
