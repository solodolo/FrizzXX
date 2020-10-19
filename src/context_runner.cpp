#include "context_runner.h"
#include "parser.h"

std::string Frizz::ContextRunner::get_namespaced_key(std::string name_space, std::string key) {
  if(name_space.empty()) {
    return key;
  }

  return name_space + ":" + key;
}

std::unordered_map<std::string, std::string> Frizz::ContextRunner::process(
  const std::string ctx_namespace,
  const std::filesystem::path file_path,
  Frizz::FileUtility& util) {
  // check cache
  auto found = this->contexts.find(file_path);
  if(found != this->contexts.end()) {
    return found->second;
  }

  Frizz::Lexer lexer;
  lexer.lex(file_path);

  std::unordered_map<std::string, std::string> context =
    this->get_main_context(ctx_namespace, lexer.get_tokens(), util);

  std::filesystem::path link = util.get_relative_content_path(file_path, true);
  link.replace_extension(".html");

  context.emplace(this->get_namespaced_key(ctx_namespace, "link"), link);

  // add to cache
  this->contexts.emplace(file_path, context);

  return context;
}

// add context from preamble and add special entry (content) for the rest of the body
std::unordered_map<std::string, std::string> Frizz::ContextRunner::get_main_context(
  std::string ctx_namespace, std::vector<Frizz::Token> tokens, Frizz::FileUtility& util) {
  std::unordered_map<std::string, std::string> context;
  Frizz::AstVisitor a_visitor;
  Frizz::Parser parser(util);

  parser.set_tokens(tokens);
  parser.parse();

  std::string content;
  for(const auto& s : parser.get_trees()) {
    std::tuple<std::string, std::string> key_val = s.get().accept(a_visitor);

    std::string key = std::get<0>(key_val);
    std::string val = std::get<1>(key_val);

    if(!key.empty()) {
      std::string namespaced_key = this->get_namespaced_key(ctx_namespace, key);
      context.emplace(namespaced_key, val);
    }
    else {
      content += val;
    }
  }

  context.emplace(this->get_namespaced_key(ctx_namespace, "content"),
                  this->convert_to_html(content));

  return context;
}

std::string Frizz::ContextRunner::convert_to_html(std::string markdown) {
  std::stringstream ss(markdown);
  return this->html_parser.Parse(ss);
}