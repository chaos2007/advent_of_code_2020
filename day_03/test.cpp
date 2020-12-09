#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <numeric>

int get_num_trees_hit(std::string file_name, int right, int down) {
  std::ifstream file(file_name);
  std::string line;
  int current_column = 0;
  int current_row = 0;
  int num_trees_hit = 0;
  while (std::getline(file, line)) {
    if (current_row == 0) {
      if (line[current_column] == '#') {
        num_trees_hit++;
      }
      current_column = (current_column + right) % line.size();
    }
    current_row++;
    if (current_row == down) current_row = 0;
  }
  return num_trees_hit;
}

TEST(Day03, sample) {
  EXPECT_EQ(get_num_trees_hit("day03_sample.txt", 3, 1), 7);
}
TEST(Day03, part_01) {
  EXPECT_EQ(get_num_trees_hit("day03_part01.txt", 3, 1), 278);
}
TEST(Day03, variable_slopes) {
  EXPECT_EQ(get_num_trees_hit("day03_sample.txt", 1, 1), 2);
  EXPECT_EQ(get_num_trees_hit("day03_sample.txt", 3, 1), 7);
  EXPECT_EQ(get_num_trees_hit("day03_sample.txt", 5, 1), 3);
  EXPECT_EQ(get_num_trees_hit("day03_sample.txt", 7, 1), 4);
  EXPECT_EQ(get_num_trees_hit("day03_sample.txt", 1, 2), 2);
}
TEST(Day03, part_02) {
  using Slope = std::pair<int, int>;
  std::vector<Slope> slopes = {{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};
  long int multiply = std::accumulate(
      slopes.begin(), slopes.end(), 1l, [](long int accu, Slope &slope) {
        return accu *
               get_num_trees_hit("day03_part01.txt", slope.first, slope.second);
      });
  EXPECT_EQ(multiply, 9709761600);
}
