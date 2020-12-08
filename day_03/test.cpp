#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>

int get_num_trees_hit(std::string file_name, int right, int down) {
  std::ifstream file(file_name);
  std::string line;
  int current_column = 0;
  int num_trees_hit = 0;
  while (std::getline(file, line)) {
    if (down == 1) {
      if (line[current_column] == '#') {
        num_trees_hit++;
      }
      current_column = (current_column + right) % line.size();
    }
  }
  return num_trees_hit;
}

TEST(Day03, sample) {
  EXPECT_EQ(get_num_trees_hit("day03_sample.txt", 3, 1), 7);
}
TEST(Day03, part_01) {
  EXPECT_EQ(get_num_trees_hit("day03_part01.txt", 3, 1), 278);
}
