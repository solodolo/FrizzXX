/*
 * expressions.h
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#ifndef SRC_EXPRESSIONS_H_
#define SRC_EXPRESSIONS_H_

#include <string>

namespace Frizz {

class AssignmentExp {
public:
	AssignmentExp(std::string name, std::string value): name(name), value(value) {};
	std::string evaluate();

private:
	bool is_src();

private:
	std::string name;
	std::string value;
};

}

#endif /* SRC_EXPRESSIONS_H_ */
