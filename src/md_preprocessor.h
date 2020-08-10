/*
 * md_preprocessor.cpp
 *
 *  Created on: Aug 6, 2020
 *      Author: dmmettlach
 */

#ifndef MD_PREPROCESSOR_CPP_
#define MD_PREPROCESSOR_CPP_

#include <sstream>

class MD_Preprocessor {
public:
	//finds replacement markers of the form @@ src=replacement.md
	//and replaces the marker with the text from replacement.md
	std::string preprocess(std::stringstream &source);

private:
	//path should not contain ".."
	bool is_valid_replacement_path(std::string path);

	//reads the file pointed to by the replacement marker and
	//returns a string of the contents
	std::string read_replacement_file(std::string path);

	//replaces a given replacement marker in source with the
	//text from the source file it points to
	void insert_replacement_file(std::string &source);
};

#endif /* MD_PREPROCESSOR_CPP_ */
