#ifndef _TESTS_TEST_HELPERS_H_
#define _TESTS_TEST_HELPERS_H_

#include <filesystem>
#include <fstream>

namespace FrizzTesting {
std::filesystem::path find_path(const std::filesystem::path& relative_path);
std::string read_file(std::filesystem::path path);
}

#endif /* _TESTS_TEST_HELPERS_H_ */