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
  Frizz::AstVisitor a_visitor;

  std::unordered_map<std::string, std::string> context;

  lexer.lex(file_path);
  parser.set_tokens(lexer.get_tokens());
  parser.parse();

  std::string content;

  for(auto const& s : parser.get_trees()) {
    std::tuple<std::string, std::string> key_val = s->accept(a_visitor);

    std::string key = std::get<0>(key_val);
    std::string val = std::get<1>(key_val);

    if(!key.empty()) {
      if(context_namespace.empty()) {
        context.emplace(key, val);
      }
      else {
        std::string namespaced_key = context_namespace + ":" + key;
        context.emplace(namespaced_key, val);
      }
    }
    else {
      content += val;
    }
  }

  context.emplace(":content", content);

  return context;
}

void Frizz::Runner::process_source_file(Frizz::Lexer& lexer,
                                        Frizz::Parser& parser,
                                        Frizz::FileUtility& util,
                                        std::filesystem::path input_path,
                                        std::filesystem::path output_path) {

  Frizz::ContextVisitor c_visitor;
  Frizz::AstVisitor a_visitor;
  Frizz::FileContentVisitor fc_visitor(util);
  Frizz::ContextChildrenVisitor children_visitor;

  std::ofstream output_stream(output_path);

  lexer.lex(input_path);
  parser.set_tokens(lexer.get_tokens());
  parser.parse();

  if(output_stream) {
    std::vector<std::shared_ptr<Frizz::BasicAst>>::const_iterator it = parser.get_trees().begin();

    for(; it != parser.get_trees().end(); ++it) {
      std::shared_ptr<BasicAst> ast = *it;

      std::vector<std::shared_ptr<Frizz::BasicAst>> children = ast->accept(children_visitor);

      if(!children.empty()) {
        for(auto& child_ptr : children) {
          std::tuple<std::string, std::filesystem::path> namespaced_context =
            child_ptr->accept(c_visitor);
          std::string context_namespace = std::get<0>(namespaced_context);
          std::filesystem::path context_path = std::get<1>(namespaced_context);

          if(!context_path.empty()) {
            std::unordered_map<std::string, std::string> context =
              this->process_partial_preamble(context_namespace, context_path, util);

            std::tuple<std::string, std::string> template_info = child_ptr->accept(a_visitor);
            std::string template_name = std::get<1>(template_info);
            std::filesystem::path template_file_path = util.get_partial_file_path(template_name);

            output_stream << this->process_with_context(template_file_path, context, util);
          }
        }
      }
      else {
        std::string file_contents = ast->accept(fc_visitor);
        std::tuple<std::string, std::string> ast_contents = ast->accept(a_visitor);

        output_stream << std::get<1>(ast_contents) << file_contents;
      }
    }

    output_stream << "\n";
  }
  else {
    if(output_stream.fail()) {
      std::cout << "Output stream " << output_path << " could not be created: " << strerror(errno)
                << std::endl;
    }
  }

  output_stream.close();
}

void Frizz::Runner::process_content_source_files(Frizz::FrizzConfig& config) {
  Frizz::FileUtility util(config);

  Frizz::Lexer lexer;
  Frizz::Parser parser(util);

  std::vector<std::filesystem::path> content_source_paths = util.get_content_source_paths();
  std::filesystem::path output_root = config.get_build_path();

  for(const auto& path : content_source_paths) {
    if(path.filename() == "_index.md") {
      // TODO: create a better method for this
      std::filesystem::path content_subpath =
        std::filesystem::relative(path, config.get_source_root_path() / "content");

      std::vector<std::filesystem::path> content_paths =
        util.get_content_file_paths(content_subpath.parent_path());

      for(const auto& content_path : content_paths) {
        std::filesystem::path output_path =
          output_root / util.get_relative_content_path(content_path);

        std::filesystem::create_directories(output_path.parent_path());

        std::ofstream output_stream(output_path);

        if(output_stream) {
          std::unordered_map<std::string, std::string> context =
            this->process_partial_preamble("", content_path, util);

          output_stream << this->process_with_context(path, context, util);
          output_stream.close();
        }
        else {
          std::cout << "Output stream " << output_path
                    << " could not be created: " << strerror(errno) << std::endl;
        }
      }
    }
  }

  // Check for _index.md file
  // If found
  //    Lookup the corresponding content in the content subfolder
  //    for each content
  //        add the content of the preamble to the context
  //        add the rest of the file's contents - preamble to context
  //        output a file with the same name as the content file using the _index.md template
}

void Frizz::Runner::process_source_files(Frizz::FrizzConfig& config) {
  Frizz::FileUtility util(config);

  Frizz::Lexer lexer;
  Frizz::Parser parser(util);

  std::vector<std::filesystem::path> source_file_paths = util.get_source_file_paths();

  for(auto path : source_file_paths) {
    std::filesystem::path rel_source_path = util.get_relative_source_path(path);
    std::filesystem::path output_path = config.get_build_path() / rel_source_path;

    std::cout << "Processing " << output_path << std::endl;

    std::filesystem::create_directories(output_path.parent_path());

    process_source_file(lexer, parser, util, path, output_path);

    lexer.clear_tokens();
    parser.clear_trees();
  }
}