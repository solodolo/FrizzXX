#include "test_helpers.h"

std::filesystem::path FrizzTesting::find_path(const std::filesystem::path& relative_path) {
  std::filesystem::path base = std::filesystem::current_path();

  while(base.has_parent_path()) {
    std::filesystem::path full = base / relative_path;

    if(std::filesystem::exists(full)) {
      return full;
    }

    base = base.parent_path();
  }

  throw std::runtime_error("Failed to find file: " + relative_path.string());
};

std::string FrizzTesting::read_file(std::filesystem::path path) {
  std::ifstream input(FrizzTesting::find_path(path));
  std::string expected;

  char c;
  while(input.get(c)) {
    expected.push_back(c);
  }

  input.close();
  
  return expected;
}