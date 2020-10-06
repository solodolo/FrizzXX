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
  return visitor.visit(*this);
}

std::string Frizz::BasicAst::accept(FileContentVisitor& visitor) {
  return visitor.visit(*this);
}

std::tuple<std::string, std::filesystem::path> Frizz::BasicAst::accept(ContextVisitor& visitor) {
  return visitor.visit(*this);
}

std::string Frizz::BasicAst::accept(ContextReplacementVisitor& visitor,
                                    std::unordered_map<std::string, std::string> context) {
  return visitor.visit(*this, context);
}

std::vector<std::shared_ptr<Frizz::BasicAst>> Frizz::BasicAst::accept(
  ContextChildrenVisitor& visitor) {
  return visitor.visit(*this);
}

/*
  ########## AssignmentAst ##########
*/
std::tuple<std::string, std::string> Frizz::AssignmentAst::accept(Frizz::AstVisitor& visitor) {
  return visitor.visit(*this);
}

std::string Frizz::AssignmentAst::accept(Frizz::FileContentVisitor& visitor) {
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
  if(this->parent == nullptr) {
    return "";
  }

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

std::vector<std::shared_ptr<Frizz::BasicAst>> Frizz::ForLoopAst::accept(
  Frizz::ContextChildrenVisitor& visitor) {
  return visitor.visit(*this);
}

std::vector<std::shared_ptr<Frizz::BasicAst>> Frizz::ForLoopAst::get_children() {
  return this->children;
}

void Frizz::ForLoopAst::add_child(std::shared_ptr<Frizz::BasicAst> child) {
  this->children.push_back(child);
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
  if(this->key.empty()) {
    return this->value;
  }
  
  return this->key + ":" + this->value;
}

std::string Frizz::CtxReplacementAst::accept(ContextReplacementVisitor& visitor,
                                             std::unordered_map<std::string, std::string> context) {

  return visitor.visit(*this, context);
}
