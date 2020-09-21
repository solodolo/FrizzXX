#include "test_helpers.h"

std::filesystem::path Frizz::find_path(const std::filesystem::path& relative_path) {
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