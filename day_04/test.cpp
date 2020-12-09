#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <numeric>

using Field = std::optional<std::string>;

bool is_valid_birth_year(std::string year) {
  if (year.size() != 4) return false;
  if (!std::all_of(year.begin(), year.end(),
                   [](auto x) { return isdigit(x); })) return false;
  auto num = std::stoi(year);
  return num >= 1920 && num <= 2002;
}

bool is_valid_issue_year(std::string year) {
  if (year.size() != 4) return false;
  if (!std::all_of(year.begin(), year.end(),
                   [](auto x) { return isdigit(x); })) return false;
  auto num = std::stoi(year);
  return num >= 2010 && num <= 2020;
}

bool is_valid_expiration_year(std::string year) {
  if (year.size() != 4) return false;
  if (!std::all_of(year.begin(), year.end(),
                   [](auto x) { return isdigit(x); })) return false;
  auto num = std::stoi(year);
  return num >= 2020 && num <= 2030;
}

bool is_valid_height(std::string height) {
  if(height.size() < 3) return false;
  std::string units = height.substr(height.size() - 2, std::string::npos);
  if (!std::all_of(height.begin(), height.end() - 2,
                   [](auto x) { return isdigit(x); }))
    return false;
  auto num = std::stoi(height.substr(0, height.size() - 2));
  if (units == "cm") {
    if (num >= 150 && num <= 193) return true;
  } else if (units == "in") {
    if (num >= 59 && num <= 76) return true;
  }
  return false;
}

bool is_valid_hair_color(std::string color) {
  if (color.size() != 7) return false;
  if (color[0] != '#') return false;
  if (!std::all_of(color.begin() + 1, color.end(),
                   [](auto x) { return isxdigit(x); }))
    return false;
  return true;
}

bool is_valid_eye_color(std::string eye_color) {
  return eye_color == "amb" || eye_color == "blu" || eye_color == "brn" ||
         eye_color == "gry" || eye_color == "grn" || eye_color == "hzl" ||
         eye_color == "oth";
}

bool is_valid_passport_id(std::string passport_id) {
  if(passport_id.size() != 9) return false;
  return std::all_of(passport_id.begin(), passport_id.end(), isdigit);
}

class Passport {
 public:
  Passport(bool strict_parsing) : strict_parsing_(strict_parsing) {}

  void add_field(std::string field) {
    // split and set.
    size_t colon = field.find(':');
    if (colon != std::string::npos) {
      std::string key = field.substr(0, colon);
      std::string value = field.substr(colon + 1, std::string::npos);
      if (key == "byr" && (!strict_parsing_ || is_valid_birth_year(value)))
        birthYear_.emplace(std::move(value));
      else if (key == "iyr" && (!strict_parsing_ || is_valid_issue_year(value)))
        issueYear_.emplace(std::move(value));
      else if (key == "eyr" && (!strict_parsing_ || is_valid_expiration_year(value)))
        expirationYear_.emplace(std::move(value));
      else if (key == "hgt" && (!strict_parsing_ || is_valid_height(value)))
        height_.emplace(std::move(value));
      else if (key == "hcl" && (!strict_parsing_ || is_valid_hair_color(value)))
        hairColor_.emplace(std::move(value));
      else if (key == "ecl" && (!strict_parsing_ || is_valid_eye_color(value)))
        eyeColor_.emplace(std::move(value));
      else if (key == "pid" && (!strict_parsing_ || is_valid_passport_id(value)))
          passportId_.emplace(std::move(value));
      else if (key == "cid")
        countryId_.emplace(std::move(value));
    }
  }

  bool is_valid() {
    return birthYear_.has_value() && issueYear_.has_value() &&
           expirationYear_.has_value() && height_.has_value() &&
           hairColor_.has_value() && eyeColor_.has_value() &&
           passportId_.has_value();
  }

  Field birthYear_;
  Field issueYear_;
  Field expirationYear_;
  Field height_;
  Field hairColor_;
  Field eyeColor_;
  Field passportId_;
  Field countryId_;

  bool strict_parsing_;
};
void do_something(std::string file_name) {
  std::ifstream file(file_name);
  std::string line;
  while (std::getline(file, line)) {
    std::cout << line.length() << std::endl;
  }
}

std::vector<Passport> parse_passports(std::string file_name,
                                      bool strict_parsing) {
  std::vector<Passport> passports;
  std::ifstream file(file_name);
  std::string line;
  std::string entry;
  while (std::getline(file, line)) {
    if (line.size() == 0) {
      // End of an entry, feed in what you found.
      Passport passport(strict_parsing);
      std::stringstream ss(entry);
      std::string field;
      while (ss >> field) {
        passport.add_field(field);
      }
      passports.push_back(std::move(passport));
      entry.clear();
    } else {
      entry.push_back(' ');
      entry.append(line);
    }
  }

  return passports;
}

TEST(Day03, CanAddFields) {
  Passport passport(false);
  EXPECT_FALSE(passport.birthYear_.has_value());
  passport.add_field("byr:2013");
  EXPECT_TRUE(passport.birthYear_.has_value());
  EXPECT_EQ(passport.birthYear_.value(), std::string("2013"));
}

TEST(Day04, test) {
  auto passports = parse_passports("day04_part01.txt", false);
  auto num_valid =
      std::count_if(passports.begin(), passports.end(),
                    [](auto passport) { return passport.is_valid(); });
  EXPECT_EQ(num_valid, 200);
}
TEST(Day04, test2) {
  auto passports = parse_passports("day04_part01.txt", true);
  auto num_valid =
      std::count_if(passports.begin(), passports.end(),
                    [](auto passport) { return passport.is_valid(); });
  EXPECT_EQ(num_valid, 116);
}

TEST(Day04, valid_birth_years) {
  EXPECT_TRUE(is_valid_birth_year("2002"));
  EXPECT_FALSE(is_valid_birth_year("2003"));
}

TEST(Day04, valid_heights) {
  EXPECT_TRUE(is_valid_height("60in"));
  EXPECT_TRUE(is_valid_height("190cm"));
  EXPECT_FALSE(is_valid_height("190in"));
  EXPECT_FALSE(is_valid_height("190"));
}

TEST(Day04, valid_hair_color) {
  EXPECT_TRUE(is_valid_hair_color("#123abc"));
  EXPECT_FALSE(is_valid_hair_color("#123abz"));
  EXPECT_FALSE(is_valid_hair_color("123abc"));
}

TEST(Day04, valid_eye_color) {
  EXPECT_TRUE(is_valid_eye_color("brn"));
  EXPECT_FALSE(is_valid_eye_color("wat"));
}

TEST(Day04, valid_passport_id) {
  EXPECT_TRUE(is_valid_passport_id("000000001"));
  EXPECT_FALSE(is_valid_passport_id("0123456789"));
}
