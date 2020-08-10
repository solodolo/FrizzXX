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
#include "md_preprocessor.h"
#include "lexer.h"

using namespace std;

std::string read_header(std::string path_to_header_file) {
	std::ifstream header_input_stream(path_to_header_file);
	std::stringstream header_text("");

	if (header_input_stream) {
		header_text << header_input_stream.rdbuf();
	}

	header_input_stream.close();
	return header_text.str();
}

void test_processing() {
	std::shared_ptr<maddy::ParserConfig> parser_config = std::make_shared<
			maddy::ParserConfig>();
	std::unique_ptr<maddy::Parser> parser = std::make_unique<maddy::Parser>(
			parser_config);

	std::ifstream markdown_input_stream("/home/dmmettlach/Documents/test.md");
	std::ofstream html_output("/home/dmmettlach/Documents/test.html",
			std::ifstream::out);

	if (markdown_input_stream && html_output) {

		std::string header_html = read_header(
				"/home/dmmettlach/Documents/header.html");

		std::stringstream markdown_text;
		markdown_text << markdown_input_stream.rdbuf();
		markdown_input_stream.close();

		std::string parsed_text = parser->Parse(markdown_text);

		html_output
				<< "<html>\n<body style='font-family: \"Lucida Sans Unicode\", \"Lucida Grande\", sans-serif'>\n";
		html_output << header_html;
		html_output << parsed_text;
		html_output << "\n</body>\n</html>";

		html_output.close();

	} else {
		if (!markdown_input_stream) {
			cout << "File not found" << endl;
		} else {
			cout << "Output can't be opened" << endl;
		}

	}
}

int main() {
	Frizz::Lexer lexer;

	std::ifstream test_input_stream("/home/dmmettlach/Documents/lexer_test.md");
	if(test_input_stream) {
		while(!test_input_stream.fail()) {
			std::cout << "Reading line " << std::endl;

			std::string line;
			std::getline(test_input_stream, line);

			lexer.set_line(line);
			lexer.next_tok();
		}
	}



	return 0;
}
