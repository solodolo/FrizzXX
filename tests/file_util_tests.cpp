/*
 * file_util_tests.cpp
 *
 *  Created on: Sept 16, 2020
 *      Author: dmmettlach
 */
#include <filesystem>
#include <gtest/gtest.h>

#include "file_utility.h"
#include "test_helpers.h"

class FileUtilityTests : public ::testing::Test {
public:
  FileUtilityTests() : util(config) {}

protected:
  Frizz::FrizzConfig config;
  Frizz::FileUtility util;
  std::filesystem::path parent_dir;

  void SetUp() override {
    this->parent_dir = Frizz::find_path("tests/test_files");
    this->config.set_parent_dir(this->parent_dir);
  }
};

TEST_F(FileUtilityTests, ValidPathToSourceSubdirReturnsRelativePath) {
  std::filesystem::path p = this->parent_dir / "sources/a/b/c";
  std::filesystem::path result = util.get_relative_source_path(p);

  EXPECT_EQ(result.string(), "a/b/c");
}

TEST_F(FileUtilityTests, InvalidPathToSourceSubdirThrowsError) {
  std::filesystem::path p("/some/other/path/sources/a/b/c");
  ASSERT_THROW(util.get_relative_source_path(p), Frizz::InvalidFilePath);
}

TEST_F(FileUtilityTests, BaseSourcePathThrowsError) {
  std::filesystem::path p("/path/to/files/sources");
  ASSERT_THROW(util.get_relative_source_path(p), Frizz::InvalidFilePath);
}