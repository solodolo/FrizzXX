/*
 * runner_tests.cpp
 *
 *  Created on: Aug 22, 2020
 *      Author: dmmettlach
 */
#include <gtest/gtest.h>

#include "processor.hpp"
#include "test_helpers.h"

class ParserTests : public ::testing::Test {
protected:
  ParserTests()
    : util(config) {}

  std::filesystem::path parent_dir;
  Frizz::FrizzConfig config;
  Frizz::FileUtility util;

  void SetUp() override {
    this->parent_dir = FrizzTesting::find_path("tests/test_files");
    config.set_parent_dir(this->parent_dir);
  }
};

TEST_F(ParserTests, ProcessPreamble) {
  std::unordered_map<std::string, std::string> map =
    process_file_preamble("", util.get_content_file_path("posts/post1.md"), util);

  auto found = map.find("test");
  EXPECT_EQ(found->second, "this is a test");
}

TEST_F(ParserTests, ProcessPreambleMultiple) {
  std::unordered_map<std::string, std::string> map =
    process_file_preamble("", util.get_content_file_path("posts/post2.md"), util);

  auto found = map.find("foo");
  EXPECT_EQ(found->second, "bar");

  found = map.find("bar");
  EXPECT_EQ(found->second, "baz");

  found = map.find("baz");
  EXPECT_EQ(found->second, "foo");
}

TEST_F(ParserTests, ProcessPreambleThrowsErrorOnNonIdents) {
  ASSERT_THROW(process_file_preamble("", util.get_content_file_path("posts/post3.md"), util),
               Frizz::ParseException);
}

TEST_F(ParserTests, ProcessPreambleIncludesContent) {
  std::unordered_map<std::string, std::string> map =
    process_file_preamble("", util.get_content_file_path("posts/post1.md"), util);

  auto found = map.find("content");
  ASSERT_NE(found, map.end());
  EXPECT_NE(map["content"], "");
}

TEST_F(ParserTests, ProcessPreambleNamespacesContent) {
  std::unordered_map<std::string, std::string> map =
    process_file_preamble("foo", util.get_content_file_path("posts/post1.md"), util);

  auto found = map.find("foo:content");
  ASSERT_NE(found, map.end());
  EXPECT_NE(map["foo:content"], "");
}

TEST_F(ParserTests, ProcessPreambleIncludesLink) {
  std::unordered_map<std::string, std::string> map =
    process_file_preamble("", util.get_content_file_path("posts/post1.md"), util);

  auto found = map.find("link");
  ASSERT_NE(found, map.end());
  EXPECT_EQ(map["link"], "content/posts/post1.html");
}

TEST_F(ParserTests, ProcessPreambleNamespacesLink) {
  std::unordered_map<std::string, std::string> map =
    process_file_preamble("bar", util.get_content_file_path("posts/post1.md"), util);

  auto found = map.find("bar:link");
  ASSERT_NE(found, map.end());
  EXPECT_EQ(map["bar:link"], "content/posts/post1.html");
}

TEST_F(ParserTests, ProcessWithContext) {
  std::string title = "This is the title of the post";

  std::unordered_map<std::string, std::string> context;
  context.emplace("post:title", title);

  std::string result = process_with_context(
    config.get_partial_templates_path() /= "contextual_partial.md", context, util);

  std::string expected = "[" + title + "](test_link)\n";
  ASSERT_EQ(result, expected);
}
