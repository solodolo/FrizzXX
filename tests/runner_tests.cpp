/*
 * runner_tests.cpp
 *
 *  Created on: Aug 22, 2020
 *      Author: dmmettlach
 */
#include <gtest/gtest.h>

#include "runner.h"
#include "test_helpers.h"

class RunnerTests : public ::testing::Test {
protected:
  RunnerTests()
    : util(config) {}

  std::filesystem::path parent_dir;
  Frizz::Runner runner;
  Frizz::FrizzConfig config;
  Frizz::FileUtility util;

  void SetUp() override {
    this->parent_dir = FrizzTesting::find_path("tests/test_files");
    config.set_parent_dir(this->parent_dir);
  }
};

TEST_F(RunnerTests, ProcessPreamble) {
  std::unordered_map<std::string, std::string> map =
    runner.process_file_preamble("", util.get_content_file_path("posts/post1.md"), util);

  auto found = map.find("test");
  EXPECT_EQ(found->second, "this is a test");
}

TEST_F(RunnerTests, ProcessPreambleMultiple) {
  std::unordered_map<std::string, std::string> map =
    runner.process_file_preamble("", util.get_content_file_path("posts/post2.md"), util);

  auto found = map.find("foo");
  EXPECT_EQ(found->second, "bar");

  found = map.find("bar");
  EXPECT_EQ(found->second, "baz");

  found = map.find("baz");
  EXPECT_EQ(found->second, "foo");
}

TEST_F(RunnerTests, ProcessPreambleThrowsErrorOnNonIdents) {
  ASSERT_THROW(
    runner.process_file_preamble("", util.get_content_file_path("posts/post3.md"), util),
    Frizz::ParseException);
}


TEST_F(RunnerTests, ProcessPreambleIncludesContent) {
  std::unordered_map<std::string, std::string> map =
    runner.process_file_preamble("", util.get_content_file_path("posts/post1.md"), util);

  auto found = map.find("content");
  ASSERT_NE(found, map.end());
  EXPECT_NE(map["content"], "");
}

TEST_F(RunnerTests, ProcessPreambleIncludesLink) {
  std::unordered_map<std::string, std::string> map =
    runner.process_file_preamble("", util.get_content_file_path("posts/post1.md"), util);

  auto found = map.find("link");
  ASSERT_NE(found, map.end());
  EXPECT_EQ(map["link"], "content/posts/post1.html");
}

TEST_F(RunnerTests, ProcessWithContext) {
  std::string title = "This is the title of the post";

  std::unordered_map<std::string, std::string> context;
  context.emplace("post:title", title);

  std::string result = runner.process_with_context(
    config.get_partial_templates_path() /= "contextual_partial.md", context, util);

  std::string expected = "[" + title + "](test_link)\n";
  ASSERT_EQ(result, expected);
}
