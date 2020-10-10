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
class FileContentVisitor;
class ContextChildrenVisitor;

// Add new ast for context replacement
class BasicAst {
public:
  virtual std::tuple<std::string, std::string> accept(AstVisitor& visitor) const;
  virtual std::string accept(FileContentVisitor& visitor) const;
  virtual std::tuple<std::string, std::filesystem::path> accept(ContextVisitor& visitor) const;
  virtual std::string accept(ContextReplacementVisitor& visitor,
                             std::unordered_map<std::string, std::string> context) const;
  virtual std::vector<std::reference_wrapper<const BasicAst>> accept(
    ContextChildrenVisitor& visitor) const;

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

  std::tuple<std::string, std::string> accept(AstVisitor& visitor) const override;
  std::vector<std::reference_wrapper<const BasicAst>> accept(
    ContextChildrenVisitor& visitor) const override;

  std::string get_key() const;
  std::string get_value() const override;

  void add_child(std::unique_ptr<BasicAst>);
  std::vector<std::reference_wrapper<const BasicAst>> get_children() const;

private:
  std::string name;
  std::string value;
  std::vector<std::unique_ptr<BasicAst>> children;
};

class AssignmentAst : public BasicAst {
public:
  AssignmentAst(std::string name, std::string value)
    : name(name)
    , value(value) {};

  std::tuple<std::string, std::string> accept(AstVisitor& visitor) const override;
  std::string accept(FileContentVisitor& visitor) const override;
  std::tuple<std::string, std::filesystem::path> accept(ContextVisitor& visitor) const override;

  std::string get_value() const override;
  std::string get_name() const;
  bool is_src() const;

  void set_namespace(std::string);
  std::string get_namespace() const;

  void set_context_filepath(std::filesystem::path path);
  std::filesystem::path get_context_filepath() const;

  void set_context(std::unordered_map<std::string, std::string> context);

private:
  std::string name_space;
  std::string name;
  std::string value;
  std::filesystem::path context_filepath;
  std::unordered_map<std::string, std::string> context;
};

class PassthroughAst : public BasicAst {
public:
  PassthroughAst(std::string value)
    : value(value) {};

  std::tuple<std::string, std::string> accept(AstVisitor& visitor) const override;
  std::string get_value() const override;

private:
  std::string value;
};

class CtxReplacementAst : public BasicAst {
public:
  CtxReplacementAst(std::string key, std::string value)
    : key(key)
    , value(value) {};

  std::string get_namespaced_key() const;
  std::string accept(ContextReplacementVisitor& visitor,
                     std::unordered_map<std::string, std::string> context) const override;

private:
  std::string key;
  std::string value;
};

class AstVisitor {
public:
  std::tuple<std::string, std::string> visit(const BasicAst& ast);
  std::tuple<std::string, std::string> visit(const AssignmentAst& ast);
  std::tuple<std::string, std::string> visit(const ForLoopAst& ast);
  std::tuple<std::string, std::string> visit(const PassthroughAst& ast);
};

class FileContentVisitor {
public:
  FileContentVisitor(Frizz::FileUtility& f_util)
    : f_util(f_util) {}

  std::string visit(const AssignmentAst& ast);
  std::string visit(const BasicAst& ast) { return ""; };

private:
  Frizz::FileUtility& f_util;
};

class ContextVisitor {
public:
  std::tuple<std::string, std::filesystem::path> visit(const AssignmentAst& ast);

  std::tuple<std::string, std::filesystem::path> visit(const BasicAst& ast) {
    std::filesystem::path empty;
    return std::make_tuple("", empty);
  };
};

class ContextReplacementVisitor {
public:
  std::string visit(const CtxReplacementAst& ast, std::unordered_map<std::string, std::string> context);
  std::string visit(const BasicAst& ast, std::unordered_map<std::string, std::string> context) {
    return ast.get_value();
  }
};

class ContextChildrenVisitor {
public:
  std::vector<std::reference_wrapper<const BasicAst>> visit(const BasicAst& ast);
  std::vector<std::reference_wrapper<const BasicAst>> visit(const ForLoopAst& ast);
};

}

#endif /* SRC_ABSTRACT_SYNTAX_TREES_H_ */
