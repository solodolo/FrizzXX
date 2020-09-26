//============================================================================
// Name        : frizz++.cpp
// Author      : Drew Mettlach
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "runner.h"
#include "frizz_config.h"

int main(int argc, char** argv) {
  int ret_val = 0;

  if(argc > 1) {
    std::string config_file_path = std::string(argv[1]);

    Frizz::FrizzConfig config;

    Frizz::Runner runner;

    if(!config.load_configuration(config_file_path)) {
      std::cout << "Invalid config file" << std::endl;
      ret_val = -1;
    }
    else {
      runner.process_content_source_files(config);
      runner.process_source_files(config);
      runner.convert_to_html(config);
    }
  }
  else {
    std::cout << "usage: FrizzXX /path/to/config.json" << std::endl;
    ret_val = -1;
  }

  return ret_val;
}
