/*
 * config_tests.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: dmmettlach
 */
#include <filesystem>
#include <gtest/gtest.h>

#include "frizz_config.h"
#include "test_helpers.h"

class ConfigTests : public ::testing::Test {
protected:
  Frizz::FrizzConfig config;
  std::filesystem::path parent_path;

  void SetUp() override {
    parent_path = FrizzTesting::find_path("tests/test_files");
    this->config.set_parent_dir(parent_path);
  }
};

TEST_F(ConfigTests, CorrectSourcePath) {
  ASSERT_EQ(config.get_source_root_path(), parent_path / "sources");
}

TEST_F(ConfigTests, CorrectPartialPath) {
  ASSERT_EQ(config.get_partial_templates_path(), parent_path / "partials");
}

TEST_F(ConfigTests, CorrectBuildPath) {
  ASSERT_EQ(config.get_build_path().string(), parent_path / "output");
}