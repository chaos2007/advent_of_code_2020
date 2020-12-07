#include <vector>
#include <optional>
#include <fstream>

#include "gtest/gtest.h"
#include "benchmark/benchmark.h"

// Keep around brute force method for looking at Benchmarking
std::optional<std::pair<int, int>> find_sum_pair_brute_force(std::vector<int> &nums, int desired_num) {
    // Do dumb way
    for( int x = 0; x < nums.size(); ++x) {
        for (int y = 0; y < nums.size(); ++y) {
            if(x == y) continue;
            if( nums[x] + nums[y] == desired_num) {
                return std::make_pair(std::min(nums[x], nums[y]), std::max(nums[x], nums[y]));
            }
        }
    }
    return {};
}

std::optional<std::pair<int, int>> find_sum_pair(std::vector<int> &nums, int desired_num) {
    int left = 0;
    int right = nums.size() - 1;

    // Sort the array.  Then we can just bring our window sizes inward if we are greater or less than our target.
    std::sort(nums.begin(), nums.end());
    while(left != right) {
        int sum = nums[left] + nums[right];
        if(sum == desired_num) {
            return std::make_pair(nums[left], nums[right]);
        } else if (sum < desired_num ) {
            left++;
        } else {
            right--;
        }
    }
    return {};
}


// TODO: Do this better, implement similar to scan method above.
std::optional<std::tuple<int, int, int>> find_sum_triplet(const std::vector<int> &nums, int desired_num) {
    // Do dumb way
    for( int x = 0; x < nums.size(); ++x) {
        for (int y = 0; y < nums.size(); ++y) {
            for (int z = 0; z < nums.size(); ++z) {
                if(x == y || y == z || x == z) continue;
                if( nums[x] + nums[y] + nums[z] == desired_num) {
                    return std::make_tuple(nums[x], nums[y], nums[z]);
                }
            }
        }
    }
    return {};
}

std::vector<int> read_file(std::string file_name) {
    std::ifstream is(file_name);
    if(is) {
        std::istream_iterator<int> start(is), end;
        std::vector<int> numbers(start, end);
        return numbers;
    } else {
        throw std::invalid_argument("File doesn't exist");
    }
}

TEST(Day01, can_find_pair) {
    std::vector<int> input = {1721, 979, 366, 299, 675, 1456};
    auto result = find_sum_pair(input, 2020);
    std::pair<int, int> pair = result.value_or(std::make_pair(0,0));
    EXPECT_EQ(pair, std::make_pair(299, 1721));
    EXPECT_EQ(pair.first * pair.second, 514579);
}

TEST(Day01, part_1) {
    auto nums = read_file("day01_part01.txt");
    auto result = find_sum_pair(nums, 2020);
    std::pair<int, int> pair = result.value_or(std::make_pair(0,0));
    EXPECT_EQ(pair, std::make_pair(317, 1703));
    EXPECT_EQ(pair.first * pair.second, 539851);
}

TEST(Day01, can_find_triplet) {
    std::vector<int> input = {1721, 979, 366, 299, 675, 1456};
    auto result = find_sum_triplet(input, 2020);
    std::tuple<int, int, int> pair = result.value_or(std::make_tuple(0,0,0));
    EXPECT_EQ(pair, std::make_tuple(979, 366, 675));
    EXPECT_EQ(std::get<0>(pair) * std::get<1>(pair) * std::get<2>(pair), 241861950);
}
TEST(Day01, part_2) {
    auto nums = read_file("day01_part01.txt");
    auto result = find_sum_triplet(nums, 2020);
    std::tuple<int, int, int> pair = result.value_or(std::make_tuple(0,0,0));
    EXPECT_EQ(pair, std::make_tuple(1081, 315, 624));
    EXPECT_EQ(std::get<0>(pair) * std::get<1>(pair) * std::get<2>(pair), 212481360);
}

static void BM_BruteForce(benchmark::State& state) {
    for (auto _ : state) {
        auto nums = read_file("day01_part01.txt");
        auto result = find_sum_pair_brute_force(nums, 2020);
    }
}
static void BM_Scan(benchmark::State& state) {
    for (auto _ : state) {
        auto nums = read_file("day01_part01.txt");
        auto result = find_sum_pair(nums, 2020);
    }
}
BENCHMARK(BM_BruteForce);
BENCHMARK(BM_Scan);
