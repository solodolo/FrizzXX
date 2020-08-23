/*
 * main_tests.cpp
 *
 *  Created on: Aug 22, 2020
 *      Author: dmmettlach
 */
#include <gtest/gtest.h>

#include "runner.h"

static const std::string TEST_DIR = "./tests/test_files/partials/posts";

class RunnerTests : public ::testing::Test {
public:
  RunnerTests()
    : util(config) {
    config.load_configuration("./tests/test_files/config/test2.json");
  }

  Frizz::Runner runner;
  Frizz::FrizzConfig config;
  Frizz::FileUtility util;
};

TEST_F(RunnerTests, ProcessPreamble) {
  std::unordered_map<std::string, std::string> map =
    runner.process_partial_preamble(config.get_partial_templates_path() /= "posts/post1.md", util);

  ASSERT_EQ(map.size(), 1);

  auto found = map.find("test");
  EXPECT_EQ(found->second, "this is a test");
}

TEST_F(RunnerTests, ProcessPreambleMultiple) {
  std::unordered_map<std::string, std::string> map =
    runner.process_partial_preamble(config.get_partial_templates_path() /= "posts/post2.md", util);

  ASSERT_EQ(map.size(), 3);

  auto found = map.find("foo");
  EXPECT_EQ(found->second, "bar");

  found = map.find("bar");
  EXPECT_EQ(found->second, "baz");

  found = map.find("baz");
  EXPECT_EQ(found->second, "foo");
}

TEST_F(RunnerTests, ProcessPreambleIgnoresNonIdents) {
  std::unordered_map<std::string, std::string> map =
    runner.process_partial_preamble(config.get_partial_templates_path() /= "posts/post3.md", util);

  ASSERT_EQ(map.size(), 1);

  auto found = map.find("is");
  EXPECT_EQ(found->second, "a block");
}