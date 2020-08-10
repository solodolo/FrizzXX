/*
 * md_preprocessor.cpp
 *
 *  Created on: Aug 6, 2020
 *      Author: dmmettlach
 */
#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "md_preprocessor.h"

//TODO: move get_replacement_directories delim to config
//TODO: allow replacement paths to directory where each item in that
// directory will get inserted as a separate copy

std::string MD_Preprocessor::preprocess(std::stringstream &source) {
	for (std::string line; getline(source, line);) {
		std::regex regex("^@{2} {1,}src=[^\n]+");
		std::smatch results;

		regex_search(line, results, regex);
		std::string result_str = results.str();

		if (!result_str.empty()) {
			size_t source_pos = result_str.find("src=", 3);
			if (source_pos != std::string::npos) {
				std::string replacement_path = result_str.substr(
						source_pos + 4);

				bool path_is_valid = this->is_valid_replacement_path(replacement_path);

				if(path_is_valid) {
					std::filesystem::path path(replacement_path);

					if(path.has_filename()) { //replace marker with the contents of this file
						std::string full_path = "/home/dmmettlach/Documents/" + path.string(); //TODO: move /home/dmmettlach/Documents to config and clean it
						std::string replacement_file_contents = this->read_replacement_file(full_path);


					}
					else {
						//for every .md file in the dir, add its contents to this one

					}
				}

				std::cout << "Is valid path " << path_is_valid << std::endl;
			}
		}
	}

	return "";
}

bool MD_Preprocessor::is_valid_replacement_path(std::string path) {
	bool valid = true;

	if(!path.empty()) {
		std::regex regex("(?:\\.\\.)");

		valid = path[0] != '/' && !std::regex_search(path, regex);
	}
	else {
		valid = false;
	}

	return valid;
}

std::string MD_Preprocessor::read_replacement_file(std::string path) {
	std::ifstream in_stream(path);
	std::stringstream ss;

	ss << in_stream.rdbuf();

	return ss.str();
}
