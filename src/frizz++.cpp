//============================================================================
// Name        : frizz++.cpp
// Author      : Drew Mettlach
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fstream>
#include <iostream>

#include "file_utility.h"
#include "frizz_config.h"
#include "lexer.h"
#include "maddy/parser.h"
#include "parser.h"

void process_source_file(Frizz::Lexer& lexer,
                         Frizz::Parser& parser,
                         Frizz::AstVisitor& visitor,
                         std::filesystem::path input_path,
                         std::filesystem::path output_path) {

  std::ifstream input_stream(input_path);
  std::ofstream output_stream(output_path);

  if(input_stream && output_stream) {
    while(!input_stream.eof()) {
      std::string line;
      std::getline(input_stream, line);

      lexer.set_line(line);
      lexer.next_tok();

      parser.set_tokens(lexer.get_tokens());
      parser.parse();

      std::vector<std::unique_ptr<Frizz::BasicAst>>::const_iterator it =
        parser.get_structures().begin();

      for(; it != parser.get_structures().end(); ++it) {
        std::string evaluated = (**it).accept(visitor);
        output_stream << evaluated << " ";
      }

      parser.clear_structures();
      output_stream << "\n";
    }
  }
  else {
    if(input_stream.fail()) {
      std::cout << "Input stream could not be created: " << strerror(errno) << std::endl;
    }

    if(output_stream.fail()) {
      std::cout << "Output stream could not be created: " << strerror(errno) << std::endl;
    }
  }

  input_stream.close();
  output_stream.close();
}

void process_source_files(Frizz::FrizzConfig& config) {

  Frizz::Lexer lexer;
  Frizz::Parser parser;

  Frizz::FileUtility util(config);
  Frizz::AstVisitor visitor(util);

  std::vector<std::filesystem::path> source_file_paths = util.get_source_file_paths();
  std::filesystem::path build_path = config.get_build_path();

  for(auto path : source_file_paths) {
    std::filesystem::path output_path = build_path /= path.filename();
    process_source_file(lexer, parser, visitor, path, output_path);
  }
}

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
      process_source_files(config);
    }
  }
  else {
    std::cout << "usage: FrizzXX /path/to/config.json" << std::endl;
    ret_val = -1;
  }

  return ret_val;
}
