/*
 * abstract_syntax_trees.h
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#ifndef SRC_ABSTRACT_SYNTAX_TREES_H_
#define SRC_ABSTRACT_SYNTAX_TREES_H_

#include <string>
#include <unordered_map>

#include "file_utility.h"

namespace Frizz {
class AstVisitor;
class ContextVisitor;
class ContextReplacementVisitor;

// Add new ast for context replacement
class BasicAst {
public:
  virtual std::tuple<std::string, std::string> accept(AstVisitor& visitor);
  virtual std::tuple<std::string, std::filesystem::path> accept(ContextVisitor& visitor);
  virtual std::string accept(ContextReplacementVisitor& visitor,
                             std::unordered_map<std::string, std::string> context);

  virtual std::string get_value() const { return this->value; }

  virtual ~BasicAst() {};

private:
  std::string value;
};

class ForLoopAst : public BasicAst {
public:
  ForLoopAst(std::string name, std::string value)
    : name(name)
    , value(value) {}

  std::tuple<std::string, std::string> accept(AstVisitor& visitor) override;

  std::string get_key();
  std::string get_value() const override;

public:
  std::vector<std::unordered_map<std::string, std::string>> context;

private:
  std::string name;
  std::string value;
};

class AssignmentAst : public BasicAst {
public:
  AssignmentAst(std::string name, std::string value)
    : name(name)
    , value(value) {};

  std::tuple<std::string, std::string> accept(AstVisitor& visitor) override;
  std::tuple<std::string, std::filesystem::path> accept(ContextVisitor& visitor) override;

  std::string get_value() const override;
  std::string get_name() const;
  bool is_src() const;

  void set_parent(std::shared_ptr<ForLoopAst> parent);
  std::string get_parent_name();

  void set_context_filepath(std::filesystem::path path);
  std::filesystem::path get_context_filepath();

  void set_context(std::unordered_map<std::string, std::string> context);

private:
  std::string name;
  std::string value;
  std::shared_ptr<ForLoopAst> parent;
  std::filesystem::path context_filepath;
  std::unordered_map<std::string, std::string> context;
};

class PassthroughAst : public BasicAst {
public:
  PassthroughAst(std::string value)
    : value(value) {};

  std::tuple<std::string, std::string> accept(AstVisitor& visitor) override;
  std::string get_value() const override;

private:
  std::string value;
};

class CtxReplacementAst : public BasicAst {
public:
  CtxReplacementAst(std::string key, std::string value)
    : key(key)
    , value(value) {};

  std::string get_namespaced_key();
  std::string accept(ContextReplacementVisitor& visitor,
                     std::unordered_map<std::string, std::string> context) override;

private:
  std::string key;
  std::string value;
};

class AstVisitor {
public:
  AstVisitor(Frizz::FileUtility& f_util)
    : f_util(f_util) {}

  std::tuple<std::string, std::string> visit(AssignmentAst& ast);
  std::tuple<std::string, std::string> visit(ForLoopAst& ast);
  std::tuple<std::string, std::string> visit(PassthroughAst& ast);

private:
  Frizz::FileUtility& f_util;
};

class ContextVisitor {
public:
  std::tuple<std::string, std::filesystem::path> visit(AssignmentAst& ast);

  std::tuple<std::string, std::filesystem::path> visit(BasicAst& ast) {
    std::filesystem::path empty;
    return std::make_tuple("", empty);
  };
};

class ContextReplacementVisitor {
public:
  std::string visit(CtxReplacementAst& ast, std::unordered_map<std::string, std::string> context);
  std::string visit(BasicAst& ast, std::unordered_map<std::string, std::string> context) {
    return ast.get_value();
  }
};

}

#endif /* SRC_ABSTRACT_SYNTAX_TREES_H_ */
