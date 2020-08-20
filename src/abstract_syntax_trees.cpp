/*
 * structures.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#include <filesystem>
#include <fstream>
#include <iostream>

#include "abstract_syntax_trees.h"

/*
  ########## AssignmentAst ##########
*/
std::string Frizz::AssignmentAst::accept(Frizz::AstVisitor& visitor) {
  return visitor.visit(*this);
}

std::unordered_map<std::string, std::string> Frizz::AssignmentAst::accept(
  Frizz::ContextVisitor& visitor) {
  return visitor.visit(*this);
}

bool Frizz::AssignmentAst::is_src() const {
  return this->name == "src";
}

std::string Frizz::AssignmentAst::get_value() const {
  return this->value;
}

/*
  ########## ForLoopAst ##########
*/

std::string Frizz::ForLoopAst::accept(Frizz::AstVisitor& visitor) {
  return visitor.visit(*this);
}

std::unordered_map<std::string, std::string> Frizz::ForLoopAst::accept(
  Frizz::ContextVisitor& visitor) {
  return visitor.visit(*this);
}

std::string Frizz::ForLoopAst::get_key() {
  return this->name;
}

std::string Frizz::ForLoopAst::get_value() {
  return this->value;
}

/*
  ########## PassthroughAst ##########
*/

std::string Frizz::PassthroughAst::accept(Frizz::AstVisitor& visitor) {
  return visitor.visit(*this);
}

std::unordered_map<std::string, std::string> Frizz::PassthroughAst::accept(
  Frizz::ContextVisitor& visitor) {
  return visitor.visit(*this);
}

std::string Frizz::PassthroughAst::get_value() const {
  return this->value;
}

/*
  ########### AstVisitor ##########
*/
std::string Frizz::AstVisitor::visit(Frizz::AssignmentAst& ast) {
  std::string contents;

  if(ast.is_src()) {
    std::string filename = ast.get_value();
    contents = this->f_util.get_partial_contents(filename);
  }

  return contents;
}

std::string Frizz::AstVisitor::visit(Frizz::ForLoopAst& ast) {
  return "";
}

std::string Frizz::AstVisitor::visit(Frizz::PassthroughAst& ast) {
  // Return the value unmodified
  return ast.get_value();
}

/*
  ########### ContextVisitor ##########
*/
std::unordered_map<std::string, std::string> Frizz::ContextVisitor::visit(Frizz::ForLoopAst& ast) {
  std::unordered_map<std::string, std::string> context;
  context.emplace(ast.get_key(), ast.get_value());

  return context;
}