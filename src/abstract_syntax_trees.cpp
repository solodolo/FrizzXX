/*
 * abstract_syntax_trees.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#include <filesystem>
#include <fstream>
#include <iostream>

#include "abstract_syntax_trees.h"
/*
  ########## BasicAst ##########
*/
std::tuple<std::string, std::string> Frizz::BasicAst::accept(AstVisitor& visitor) {
  return std::make_tuple("", "");
}

std::tuple<std::string, std::filesystem::path> Frizz::BasicAst::accept(ContextVisitor& visitor) {
  return visitor.visit(*this);
}

std::string Frizz::BasicAst::accept(ContextReplacementVisitor& visitor,
                                    std::unordered_map<std::string, std::string> context) {
  return visitor.visit(*this, context);
}

/*
  ########## AssignmentAst ##########
*/
std::tuple<std::string, std::string> Frizz::AssignmentAst::accept(Frizz::AstVisitor& visitor) {
  return visitor.visit(*this);
}

std::tuple<std::string, std::filesystem::path> Frizz::AssignmentAst::accept(
  Frizz::ContextVisitor& visitor) {
  return visitor.visit(*this);
}

bool Frizz::AssignmentAst::is_src() const {
  return this->get_name() == "src";
}

std::string Frizz::AssignmentAst::get_name() const {
  return this->name;
}

std::string Frizz::AssignmentAst::get_value() const {
  return this->value;
}

void Frizz::AssignmentAst::set_parent(std::shared_ptr<ForLoopAst> parent) {
  this->parent = std::move(parent);
}

std::string Frizz::AssignmentAst::get_parent_name() {
  return this->parent->get_key();
}

void Frizz::AssignmentAst::set_context_filepath(std::filesystem::path path) {
  this->context_filepath = path;
}

std::filesystem::path Frizz::AssignmentAst::get_context_filepath() {
  return this->context_filepath;
}

void Frizz::AssignmentAst::set_context(std::unordered_map<std::string, std::string> context) {
  this->context = context;
}

/*
  ########## ForLoopAst ##########
*/
std::tuple<std::string, std::string> Frizz::ForLoopAst::accept(Frizz::AstVisitor& visitor) {
  return visitor.visit(*this);
}

std::string Frizz::ForLoopAst::get_key() const {
  return this->name;
}

std::string Frizz::ForLoopAst::get_value() const {
  return this->value;
}

/*
  ########## PassthroughAst ##########
*/

std::tuple<std::string, std::string> Frizz::PassthroughAst::accept(Frizz::AstVisitor& visitor) {
  return visitor.visit(*this);
}

std::string Frizz::PassthroughAst::get_value() const {
  return this->value;
}

/*
  ########## CtxReplacementAst
*/

std::string Frizz::CtxReplacementAst::get_namespaced_key() {
  return this->key + ":" + this->value;
}

std::string Frizz::CtxReplacementAst::accept(ContextReplacementVisitor& visitor,
                                             std::unordered_map<std::string, std::string> context) {

  return visitor.visit(*this, context);
}

/*
  ########### AstVisitor ##########
*/
std::tuple<std::string, std::string> Frizz::AstVisitor::visit(Frizz::AssignmentAst& ast) {
  std::string contents;
  std::string name;

  if(ast.is_src()) {
    std::string filename = ast.get_value();
    contents = this->f_util.get_partial_contents(filename);
  }
  else {
    contents = ast.get_value();
  }

  return std::make_tuple(ast.get_name(), contents);
}

std::tuple<std::string, std::string> Frizz::AstVisitor::visit(Frizz::ForLoopAst& ast) {
  return std::make_tuple("", "");
}

std::tuple<std::string, std::string> Frizz::AstVisitor::visit(Frizz::PassthroughAst& ast) {
  // Return the value unmodified
  return std::make_tuple("", ast.get_value());
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