#ifndef _TESTS_TEST_HELPERS_H_
#define _TESTS_TEST_HELPERS_H_

#include <filesystem>

namespace Frizz {
std::filesystem::path find_path(const std::filesystem::path& relative_path);
}

#endif /* _TESTS_TEST_HELPERS_H_ */