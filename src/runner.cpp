/*
 * runner.cpp
 *
 *  Created on: Aug 22, 2020
 *      Author: dmmettlach
 */

#include <cstring>
#include <fstream>
#include <iostream>

#include "runner.h"

std::string Frizz::Runner::process_with_context(
  std::filesystem::path file_path,
  std::unordered_map<std::string, std::string> context,
  Frizz::FileUtility& util) {

  Frizz::Lexer lexer;
  Frizz::Parser parser(util);
  Frizz::ContextReplacementVisitor visitor;

  lexer.lex(file_path);
  parser.set_tokens(lexer.get_tokens());
  parser.parse();

  std::string replaced;

  for(auto const& s : parser.get_trees()) {
    replaced += s->accept(visitor, context);
  }

  return replaced;
}

std::unordered_map<std::string, std::string> Frizz::Runner::process_partial_preamble(
  std::string context_namespace, std::filesystem::path file_path, Frizz::FileUtility& util) {

  Frizz::Lexer lexer;
  Frizz::Parser parser(util);
  Frizz::AstVisitor a_visitor(util);

  std::unordered_map<std::string, std::string> context;

  lexer.lex(file_path);
  parser.set_tokens(lexer.get_tokens());
  parser.parse();

  for(auto const& s : parser.get_trees()) {
    std::tuple<std::string, std::string> key_val = s->accept(a_visitor);

    std::string key = std::get<0>(key_val);
    std::string val = std::get<1>(key_val);

    if(!key.empty() && !val.empty()) {
      if(context_namespace.empty()) {
        context.emplace(key, val);
      }
      else {
        std::string namespaced_key = context_namespace + ":" + key;
        context.emplace(namespaced_key, val);
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

  Frizz::ContextVisitor c_visitor;
  Frizz::AstVisitor a_visitor(util);

  std::ofstream output_stream(output_path);

  lexer.lex(input_path);
  parser.set_tokens(lexer.get_tokens());
  parser.parse();

  if(output_stream) {
    std::vector<std::shared_ptr<Frizz::BasicAst>>::const_iterator it = parser.get_trees().begin();

    for(; it != parser.get_trees().end(); ++it) {
      std::tuple<std::string, std::filesystem::path> namespaced_context = (**it).accept(c_visitor);
      std::string context_namespace = std::get<0>(namespaced_context);
      std::filesystem::path context_path = std::get<1>(namespaced_context);

      if(!context_path.empty()) {
        std::unordered_map<std::string, std::string> context =
          this->process_partial_preamble(context_namespace, context_path, util);

        std::tuple<std::string, std::string> template_info = (**it).accept(a_visitor);
        std::string template_name = std::get<1>(template_info);
        std::filesystem::path template_file_path = util.get_partial_file_path(template_name);
        
        output_stream << this->process_with_context(template_file_path, context, util);
      }

      std::string evaluated = std::get<1>((**it).accept(a_visitor));
      output_stream << evaluated << " ";
    }

    output_stream << "\n";
  }
  else {
    if(output_stream.fail()) {
      std::cout << "Output stream could not be created: " << strerror(errno) << std::endl;
    }
  }

  output_stream.close();
}

void Frizz::Runner::process_source_files(Frizz::FrizzConfig& config) {
  Frizz::FileUtility util(config);

  Frizz::Lexer lexer;
  Frizz::Parser parser(util);

  std::vector<std::filesystem::path> source_file_paths = util.get_source_file_paths();

  for(auto path : source_file_paths) {
    std::filesystem::path output_path = config.get_build_path() /= path.filename();
    process_source_file(lexer, parser, util, path, output_path);
  }
}