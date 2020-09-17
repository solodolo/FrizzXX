/*
 * file_util_tests.cpp
 *
 *  Created on: Sept 16, 2020
 *      Author: dmmettlach
 */
#include <filesystem>
#include <gtest/gtest.h>

#include "file_utility.h"

static const std::string TEST_FILE_DIR = "./tests/test_files/config";

class FileUtilityTests : public ::testing::Test {
public:
  FileUtilityTests() : util(config) {}

protected:
  Frizz::FrizzConfig config;
  Frizz::FileUtility util;

  void SetUp() override {
    std::string config_path = TEST_FILE_DIR + "/test1.json";
    this->config.load_configuration(config_path);
  }
};

TEST_F(FileUtilityTests, ValidPathToSourceSubdirReturnsRelativePath) {
  std::filesystem::path p("/path/to/files/sources/a/b/c");
  std::filesystem::path result = util.get_relative_source_path(p);

  EXPECT_EQ(result.string(), "a/b/c");
}

TEST_F(FileUtilityTests, InvalidPathToSourceSubdirThrowsError) {
  std::filesystem::path p("/some/other/path/sources/a/b/c");
  ASSERT_THROW(util.get_relative_source_path(p), Frizz::InvalidFilePath);
}

TEST_F(FileUtilityTests, BaseSourcePathReturnsDot) {
  std::filesystem::path p("/path/to/files/sources");
  std::filesystem::path result = util.get_relative_source_path(p);

  EXPECT_EQ(result.string(), ".");
}