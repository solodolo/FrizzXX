//============================================================================
// Name        : frizz++.cpp
// Author      : Drew Mettlach
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>

#include "maddy/parser.h"
#include "frizz_config.h"
#include "lexer.h"

using namespace std;

int main(int argc, char **argv) {
	if(argc > 1) {
		std::string config_file_path = std::string(argv[1]);
	
		Frizz::FrizzConfig config;
		config.load_configuration(config_file_path);
		std::ifstream test_input_stream(config.get_static_files_root());

		Frizz::Lexer lexer;

		if(test_input_stream) {
			while(!test_input_stream.fail()) {
				std::cout << "Reading line " << std::endl;

				std::string line;
				std::getline(test_input_stream, line);

				lexer.set_line(line);
				lexer.next_tok();
			}
		}
	}
	
	return 0;
}
