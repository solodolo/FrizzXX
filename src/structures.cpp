/*
 * structures.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: dmmettlach
 */

#include <fstream>
#include <filesystem>

#include "structures.h"

bool Frizz::AssignmentStruct::is_src() {
	return this->name == "src";
}
std::string Frizz::AssignmentStruct::evaluate(std::string root_path) {
	if(this->is_src()) {
		
	}
}
