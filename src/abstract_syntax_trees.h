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

class BasicAst {
public:
  virtual std::tuple<std::string, std::string> accept(AstVisitor& visitor) = 0;
  virtual std::vector<std::filesystem::path> accept(ContextVisitor& visitor) = 0;
  virtual std::string get_value() const { return this->value; }

  virtual ~BasicAst() {};

private:
  std::string value;
};

class AssignmentAst : public BasicAst {
public:
  AssignmentAst(std::string name, std::string value)
    : name(name)
    , value(value) {};

  std::tuple<std::string, std::string> accept(AstVisitor& visitor) override;
  std::vector<std::filesystem::path> accept(ContextVisitor& visitor) override;

  std::string get_value() const override;
  std::string get_name() const;
  bool is_src() const;

  void set_context(std::unordered_map<std::string, std::string> context);

private:
  std::string name;
  std::string value;
  std::unordered_map<std::string, std::string> context;
};

class ForLoopAst : public BasicAst {
public:
  ForLoopAst(std::string name, std::string value)
    : name(name)
    , value(value) {}

  std::tuple<std::string, std::string> accept(AstVisitor& visitor) override;
  std::vector<std::filesystem::path> accept(ContextVisitor& visitor) override;

  std::string get_key();
  std::string get_value() const override;

private:
  std::string name;
  std::string value;
};

class PassthroughAst : public BasicAst {
public:
  PassthroughAst(std::string value)
    : value(value) {};

  std::tuple<std::string, std::string> accept(AstVisitor& visitor) override;
  std::vector<std::filesystem::path> accept(ContextVisitor& visitor) override;
  std::string get_value() const override;

private:
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
  ContextVisitor(Frizz::FileUtility& f_util)
    : f_util(f_util) {};

  std::vector<std::filesystem::path> visit(AssignmentAst& ast) {
    std::vector<std::filesystem::path> paths;
    return paths;
  };

  std::vector<std::filesystem::path> visit(PassthroughAst& ast) {
    std::vector<std::filesystem::path> paths;
    return paths;
  };

  std::vector<std::filesystem::path> visit(ForLoopAst& ast);

private:
  Frizz::FileUtility& f_util;
};

}

#endif /* SRC_ABSTRACT_SYNTAX_TREES_H_ */
