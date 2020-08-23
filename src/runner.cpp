/*
 * runner.cpp
 *
 *  Created on: Aug 22, 2020
 *      Author: dmmettlach
 */

#include <fstream>
#include <iostream>
#include <cstring>

#include "runner.h"

std::unordered_map<std::string, std::string> Frizz::Runner::process_partial_preamble(
  std::filesystem::path file_path, Frizz::FileUtility& util) {

  Frizz::Lexer lexer;
  Frizz::Parser parser;
  Frizz::AstVisitor a_visitor(util);

  std::ifstream input_stream(file_path);

  std::unordered_map<std::string, std::string> context;

  if(input_stream) {
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
        std::tuple<std::string, std::string> key_val = (**it).accept(a_visitor);
        
        std::string key = std::get<0>(key_val);
        std::string val = std::get<1>(key_val);

        if(!key.empty() && !val.empty()) {
          context.emplace(key, val);
        }
      }
    }
  }

  return context;
}

void Frizz::Runner::process_source_file(Frizz::Lexer& lexer,
                         Frizz::Parser& parser,
                         Frizz::FileUtility& util,
                         std::filesystem::path input_path,
                         std::filesystem::path output_path) {

  Frizz::ContextVisitor c_visitor(util);
  Frizz::AstVisitor a_visitor(util);

  std::ifstream input_stream(input_path);
  std::ofstream output_stream(output_path);

  std::vector<std::unordered_map<std::string, std::string>> context;

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
        std::vector<std::filesystem::path> paths = (**it).accept(c_visitor);

        for(auto it = paths.begin(); it != paths.end(); ++it) {
          context.push_back(process_partial_preamble(*it, util));
        }

        for(auto it = context.begin(); it != context.end(); ++it) {
          
        }

        std::string evaluated = std::get<1>((**it).accept(a_visitor));
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

void Frizz::Runner::process_source_files(Frizz::FrizzConfig& config) {

  Frizz::Lexer lexer;
  Frizz::Parser parser;

  Frizz::FileUtility util(config);

  std::vector<std::filesystem::path> source_file_paths = util.get_source_file_paths();

  for(auto path : source_file_paths) {
    std::filesystem::path output_path = config.get_build_path() /= path.filename();
    process_source_file(lexer, parser, util, path, output_path);
  }
}