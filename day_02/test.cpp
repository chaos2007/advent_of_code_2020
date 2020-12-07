#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>

struct PasswordConfig {
    int min;
    int max;
    char character;
    std::string password;
};

bool is_valid_password(int min, int max, char character, std::string password) {
    size_t count = std::count(password.begin(), password.end(), character);
    return (count >= min && count <= max);
}

bool is_valid_password_2(int min, int max, char character, std::string password) {
    return (password[min-1] == character) ^ (password[max - 1] == character);
}

PasswordConfig convert_password_config(std::string password_config) {
    size_t hypen_index = password_config.find('-');
    size_t colon_index = password_config.find(':');
    PasswordConfig ret = {.min = std::stoi(password_config.substr(0, hypen_index)),
                          .max = std::stoi(password_config.substr(hypen_index+1, colon_index - hypen_index)),
                          .character = password_config[colon_index-1],
                          .password = password_config.substr(colon_index + 2, std::string::npos)
    };
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
    EXPECT_TRUE(is_valid_password(1, 3, 'a', "abcde"));
    EXPECT_TRUE(is_valid_password(2, 9, 'c', "ccccccccc"));
}

TEST(Day02, under_min_invalid) {
    EXPECT_FALSE(is_valid_password(1, 3, 'a', "bbcde"));
}

TEST(Day02, over_max_invalid) {
    EXPECT_FALSE(is_valid_password(1, 3, 'a', "aaaae"));
}

// TODO: Cleanup the conversion process.
TEST(Day02, part_01) {
    std::ifstream file("day02_part01.txt");
    std::string line;
    auto sum = 0;
    while(std::getline(file, line)) {
        auto password_config = convert_password_config(line);
        if(is_valid_password(password_config.min, password_config.max, password_config.character, password_config.password)) {
            sum++;
        }
    }
    EXPECT_EQ(sum, 603);
}

TEST(Day02, valid_password_2) {
    EXPECT_TRUE(is_valid_password_2(1, 3, 'a', "abcde"));
    EXPECT_FALSE(is_valid_password_2(1, 3, 'b', "cdefg"));
    EXPECT_FALSE(is_valid_password_2(2, 9, 'c', "ccccccccc"));
}

TEST(Day02, part_02) {
    std::ifstream file("day02_part01.txt");
    std::string line;
    auto sum = 0;
    while(std::getline(file, line)) {
        auto password_config = convert_password_config(line);
        if(is_valid_password_2(password_config.min, password_config.max, password_config.character, password_config.password)) {
            sum++;
        }
    }
    EXPECT_EQ(sum, 404);
}
