#include "context_parser.h"
#include "parser.h"

void Frizz::ContextParser::set_tokens(std::vector<Frizz::Token> tokens) {
  this->tokens = tokens;

  if(this->tokens.back().id != TokType::tok_eof) {
    Token eof(TokType::tok_eof);
    this->tokens.push_back(eof);
  }
}

std::unordered_map<std::string, std::string> Frizz::ContextParser::parse(
  // check cache
  std::string ctx_namespace, std::filesystem::path file_path) {
  auto found = this->contexts.find(file_path);
  if(found != this->contexts.end()) {
    return found->second;
  }

  std::unordered_map<std::string, std::string> context = this->get_main_context(ctx_namespace);

  std::filesystem::path link = util.get_relative_content_path(file_path, true);
  link.replace_extension(".html");

  context.emplace("link", link);

  // add to cache
  this->contexts.emplace(file_path, context);

  return context;
}

// add context from preamble and add special entry (content) for the rest of the body
std::unordered_map<std::string, std::string> Frizz::ContextParser::get_main_context(
  std::string ctx_namespace) {
  std::unordered_map<std::string, std::string> context;
  Frizz::AstVisitor a_visitor;
  Frizz::Parser parser(this->util);

  parser.parse();

  std::string content;
  for(auto const& s : parser.get_trees()) {
    std::tuple<std::string, std::string> key_val = s->accept(a_visitor);

    std::string key = std::get<0>(key_val);
    std::string val = std::get<1>(key_val);

    if(!key.empty()) {
      if(ctx_namespace.empty()) {
        context.emplace(key, val);
      }
      else {
        std::string namespaced_key = ctx_namespace + ":" + key;
        context.emplace(namespaced_key, val);
      }
    }
    else {
      content += val;
    }
  }

  context.emplace("content", this->convert_to_html(content));

  return context;
}

std::string Frizz::ContextParser::convert_to_html(std::string markdown) {
  std::stringstream ss(markdown);
  return this->html_parser.Parse(ss);
}