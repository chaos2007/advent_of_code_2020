#include <gtest/gtest.h>

#include <algorithm>
#include <fstream>
#include <numeric>

class BoardingPass {
 public:
  BoardingPass(std::string pass_string) {
    // Transform each character to correct digit;
    std::transform(pass_string.begin(), pass_string.end(), pass_string.begin(),
                   [](uint8_t character) {
                     if (character == 'B' || character == 'R') {
                       return '1';
                     } else {
                       return '0';
                     }
                   });
    char *end;
    seat_id_ = strtol(pass_string.c_str(), &end, 2);
  }

  uint32_t get_row_num() { return seat_id_ >> 3; }
  uint32_t get_column_num() { return seat_id_ & 0x7; }
  uint32_t get_seat_id() { return seat_id_; }

 private:
  uint32_t seat_id_;
};

TEST(Day05, CanGetSeatId) {
  EXPECT_EQ(BoardingPass("BFFFBBFRRR").get_seat_id(), 567u);
  EXPECT_EQ(BoardingPass("FFFBBBFRRR").get_seat_id(), 119u);
  EXPECT_EQ(BoardingPass("BBFFBBFRLL").get_seat_id(), 820u);
}

TEST(Day05, CanGetRowNum) {
  EXPECT_EQ(BoardingPass("BFFFBBFRRR").get_row_num(), 70u);
  EXPECT_EQ(BoardingPass("FFFBBBFRRR").get_row_num(), 14u);
  EXPECT_EQ(BoardingPass("BBFFBBFRLL").get_row_num(), 102u);
}

TEST(Day05, CanGetColNum) {
  EXPECT_EQ(BoardingPass("BFFFBBFRRR").get_column_num(), 7u);
  EXPECT_EQ(BoardingPass("FFFBBBFRRR").get_column_num(), 7u);
  EXPECT_EQ(BoardingPass("BBFFBBFRLL").get_column_num(), 4u);
}

TEST(Day05, Part01) {
  std::ifstream file("day05_part01.txt");
  std::string line;
  auto max = 0u;
  while (std::getline(file, line)) {
    BoardingPass pass(line);
    max = std::max(pass.get_seat_id(), max);
  }
  EXPECT_EQ(max, 850u);
}
TEST(Day05, Part02) {
  std::vector<BoardingPass> passes;
  std::ifstream file("day05_part01.txt");
  std::string line;
  while (std::getline(file, line)) {
    passes.emplace_back(BoardingPass(line));
  }
  std::sort(passes.begin(), passes.end(),
            [](auto &x, auto &y) { return x.get_seat_id() < y.get_seat_id(); });
  auto seat_num = passes.begin()->get_seat_id();
  for(auto & pass : passes) {
    if(seat_num != pass.get_seat_id()) break;
    seat_num++;
  }
  EXPECT_EQ(seat_num, 599u);
}
