/*
 * file_utility.cpp
 *
 *  Created on: Aug 16, 2020
 *      Author: dmmettlach
 */

#include <fstream>
#include <iostream>

#include "file_utility.h"

void Frizz::FileUtility::process_source_files(std::filesystem::path source_files_path,
                                              std::filesystem::path replacement_lookup_path,
                                              std::filesystem::path build_path) {
  std::filesystem::directory_iterator source_it(source_files_path);
  for(auto& entry : source_it) {
    std::filesystem::path path = entry.path();
    if(path.extension() != ".md") {
      continue;
    }

    std::filesystem::path output_file_path = build_path;
    output_file_path /= path.filename();

    this->process_source_file(path, output_file_path);
  }
}

void Frizz::FileUtility::process_source_file(std::filesystem::path file_path,
                                             std::filesystem::path replacement_lookup_path,
                                             std::filesystem::path output_file_path) {

  std::ifstream input_stream(file_path);
  std::ofstream output_stream(output_file_path);

  if(input_stream && output_stream) {
    while(!input_stream.eof()) {
      std::string line;
      std::getline(input_stream, line);

      this->lexer.set_line(line);
      this->lexer.next_tok();

      this->parser.set_tokens(lexer.get_tokens());
      this->parser.parse();

      std::vector<std::unique_ptr<Frizz::BasicStruct>>::const_iterator it =
        this->parser.get_structures().begin();

      for(; it != this->parser.get_structures().end(); ++it) {
        std::string evaluated = (**it).evaluate(replacement_lookup_path);
        output_stream << evaluated << " ";
      }

      this->parser.clear_structures();
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