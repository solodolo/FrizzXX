/*
 * syntax_tree_visitors.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: dmmettlach
 */

#include "abstract_syntax_trees.h"

/*
  ########### AstVisitor ##########
*/

std::tuple<std::string, std::string> Frizz::AstVisitor::visit(Frizz::BasicAst& ast) {
  return std::make_tuple("", "");
}

std::tuple<std::string, std::string> Frizz::AstVisitor::visit(Frizz::AssignmentAst& ast) {
  // src="blah.md" should be used to place a partial as-is inline
  // therefore src assignments should only return info when accessed by FileContentVisitor
  if(ast.is_src()) {
    return std::make_tuple("", "");
  }
  
  // not a source so return key and value as-is
  return std::make_tuple(ast.get_name(), ast.get_value());
}

std::tuple<std::string, std::string> Frizz::AstVisitor::visit(Frizz::ForLoopAst& ast) {
  return std::make_tuple("", "");
}

std::tuple<std::string, std::string> Frizz::AstVisitor::visit(Frizz::PassthroughAst& ast) {
  // Return the value unmodified
  return std::make_tuple("", ast.get_value());
}

/*
  ########### FileContentVisitor ##########
*/
std::string Frizz::FileContentVisitor::visit(Frizz::AssignmentAst& ast) {
  std::string contents;

  if(ast.is_src()) {
    std::string filename = ast.get_value();
    contents = this->f_util.get_partial_contents(filename);
  }

  return contents;
}

/*
  ########### ContextVisitor ##########
*/
std::tuple<std::string, std::filesystem::path> Frizz::ContextVisitor::visit(
  Frizz::AssignmentAst& ast) {

  std::string parent_name = ast.get_parent_name();
  std::filesystem::path context_filepath = ast.get_context_filepath();

  return std::make_tuple(parent_name, context_filepath);
}

/*
  ########## ContextReplacementVisitor ##########
*/
std::string Frizz::ContextReplacementVisitor::visit(
  CtxReplacementAst& ast, std::unordered_map<std::string, std::string> context) {
  std::string lookup_key = ast.get_namespaced_key();
  auto got = context.find(lookup_key);

  if(got == context.end()) {
    return "";
  }

  return got->second;
}

/*
  ########## ContextChildrenVisitor ##########
*/
std::vector<std::shared_ptr<Frizz::BasicAst>> Frizz::ContextChildrenVisitor::visit(
  Frizz::BasicAst& ast) {
  return std::vector<std::shared_ptr<Frizz::BasicAst>>();
}

std::vector<std::shared_ptr<Frizz::BasicAst>> Frizz::ContextChildrenVisitor::visit(
  Frizz::ForLoopAst& ast) {

  return ast.get_children();
}