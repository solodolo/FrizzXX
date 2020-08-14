/*
 * expressions.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#include <fstream>
#include <filesystem>

#include "expressions.h"

bool Frizz::AssignmentExp::is_src() {
	return this->name == "src";
}
std::string Frizz::AssignmentExp::evaluate(std::string root_path) {
	if(this->is_src()) {
		
	}
}
