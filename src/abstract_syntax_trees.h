/*
 * abstract_syntax_trees.h
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#ifndef SRC_ABSTRACT_SYNTAX_TREES_H_
#define SRC_ABSTRACT_SYNTAX_TREES_H_

#include <string>

#include "file_utility.h"

namespace Frizz {
class AstVisitor;

class BasicAst {
public:
  virtual std::string accept(AstVisitor& visitor) = 0;
  virtual ~BasicAst() {};
};

class AssignmentAst : public BasicAst {
public:
  AssignmentAst(std::string name, std::string value)
    : name(name)
    , value(value) {};

  std::string accept(AstVisitor& visitor) override;
  std::string get_value() const;
  bool is_src() const;

private:
  std::string name;
  std::string value;
};

class PassthroughAst : public BasicAst {
public:
  PassthroughAst(std::string value)
    : value(value) {};

  std::string accept(AstVisitor& visitor) override;
  std::string get_value() const;

private:
  std::string value;
};

class AstVisitor {
public:
  AstVisitor(Frizz::FileUtility& f_util)
    : f_util(f_util) {}
  std::string visit(AssignmentAst& ast);
  std::string visit(PassthroughAst& ast);

private:
  Frizz::FileUtility& f_util;
};
}

#endif /* SRC_ABSTRACT_SYNTAX_TREES_H_ */
