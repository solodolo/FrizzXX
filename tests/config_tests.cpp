/*
 * config_tests.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: dmmettlach
 */
#include <filesystem>
#include <gtest/gtest.h>

#include "frizz_config.h"

static const std::string TEST_FILE_DIR = "./tests/test_files/config";

class ConfigTests : public ::testing::Test {
protected:
  Frizz::FrizzConfig config;

  void SetUp() override {
    std::string config_path = TEST_FILE_DIR + "/test1.json";
    this->config.load_configuration(config_path);
  }
};

TEST_F(ConfigTests, CorrectSourcePath) {
  ASSERT_EQ(config.get_source_root_path().string(), "/path/to/root");
}

TEST_F(ConfigTests, CorrectBuildPath) {
  ASSERT_EQ(config.get_build_path().string(), "/path/to/build/dir");
}