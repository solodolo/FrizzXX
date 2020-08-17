/*
 * structures.h
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#ifndef SRC_STRUCTURES_H_
#define SRC_STRUCTURES_H_

#include <string>

namespace Frizz {

class BasicAst {
public:
  std::string evaluate(const std::string& root_path) { return this->do_evaluate(root_path); };
  virtual ~BasicAst() {};

private:
  virtual std::string do_evaluate(const std::string& root_path) = 0;
};

class AssignmentAst : public BasicAst {
public:
  AssignmentAst(std::string name, std::string value)
    : name(name)
    , value(value) {};

private:
  bool is_src();
  std::string do_evaluate(const std::string& root_path) override;
  std::string load_from_file(const std::string& root_path);

private:
  std::string name;
  std::string value;
};

class PassthroughStruct : public BasicAst {
public:
  PassthroughStruct(std::string value)
    : value(value) {};

private:
  std::string value;
  std::string do_evaluate(const std::string& root_path) override;
};

}

#endif /* SRC_STRUCTURES_H_ */
