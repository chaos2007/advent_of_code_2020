#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>

struct PasswordConfig {
  int min;
  int max;
  char character;
  std::string password;
};

bool is_valid_password(const PasswordConfig &passwordConfig) {
  auto &password = passwordConfig.password;
  int count =
      std::count(password.begin(), password.end(), passwordConfig.character);
  return (count >= passwordConfig.min && count <= passwordConfig.max);
}

bool is_valid_password_2(const PasswordConfig &passwordConfig) {
  auto &password = passwordConfig.password;
  auto &character = passwordConfig.character;
  return (password[passwordConfig.min - 1] == character) ^
         (password[passwordConfig.max - 1] == character);
}

PasswordConfig convert_password_config(const std::string &password_config) {
  size_t hypen_index = password_config.find('-');
  size_t colon_index = password_config.find(':');
  PasswordConfig ret = {
      .min = std::stoi(password_config.substr(0, hypen_index)),
      .max = std::stoi(
          password_config.substr(hypen_index + 1, colon_index - hypen_index)),
      .character = password_config[colon_index - 1],
      .password = password_config.substr(colon_index + 2, std::string::npos)};
  return ret;
}

TEST(Day02, can_parse_password_line) {
  PasswordConfig password_config = convert_password_config("1-3 a: abcde");
  EXPECT_EQ(password_config.min, 1);
  EXPECT_EQ(password_config.max, 3);
  EXPECT_EQ(password_config.character, 'a');
  EXPECT_EQ(password_config.password, std::string("abcde"));
}

TEST(Day02, valid_password) {
  PasswordConfig config_1 = {
      .min = 1, .max = 3, .character = 'a', .password = std::string("abcde")};
  EXPECT_TRUE(is_valid_password(config_1));
  PasswordConfig config_2 = {
      .min = 1, .max = 3, .character = 'a', .password = std::string("abcde")};
  EXPECT_TRUE(is_valid_password(config_2));
}

TEST(Day02, under_min_invalid) {
  PasswordConfig config = {
      .min = 1, .max = 3, .character = 'a', .password = std::string("bbcde")};
  EXPECT_FALSE(is_valid_password(config));
}

TEST(Day02, over_max_invalid) {
  PasswordConfig config = {
      .min = 1, .max = 3, .character = 'a', .password = std::string("aaaae")};
  EXPECT_FALSE(is_valid_password(config));
}

TEST(Day02, part_01) {
  std::ifstream file("day02_part01.txt");
  std::string line;
  auto sum = 0;
  while (std::getline(file, line)) {
    auto password_config = convert_password_config(line);
    if (is_valid_password(password_config)) {
      sum++;
    }
  }
  EXPECT_EQ(sum, 603);
}

TEST(Day02, valid_password_2) {
  PasswordConfig config_1 = {
      .min = 1, .max = 3, .character = 'a', .password = std::string("abcde")};
  EXPECT_TRUE(is_valid_password_2(config_1));
  PasswordConfig config_2 = {
      .min = 1, .max = 3, .character = 'b', .password = std::string("cdefg")};
  EXPECT_FALSE(is_valid_password_2(config_2));
  PasswordConfig config_3 = {.min = 2,
                             .max = 9,
                             .character = 'c',
                             .password = std::string("ccccccccc")};
  EXPECT_FALSE(is_valid_password_2(config_3));
}

TEST(Day02, part_02) {
  std::ifstream file("day02_part01.txt");
  std::string line;
  auto sum = 0;
  while (std::getline(file, line)) {
    auto password_config = convert_password_config(line);
    if (is_valid_password_2(password_config)) {
      sum++;
    }
  }
  EXPECT_EQ(sum, 404);
}
