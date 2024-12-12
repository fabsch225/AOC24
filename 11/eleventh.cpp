//
// Created by fabian on 12/11/24.
//
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

std::pair<long, long> split(long num) {
    int digits = std::log10(num) + 1;
    int half_digits = digits / 2;
    long divisor = std::pow(10, half_digits);
    long left = num / divisor;
    long right = num % divisor;
    return {left, right};
}

int main() {
    //4329, 385, 0, 1444386, 600463, 19, 1, 56615
    std::unordered_map<long, long> counts = {
        {4329, 1},
        {385, 1},
        {0, 1},
        {1444386, 1},
        {600463, 1},
        {19, 1},
        {1, 1},
        {56615, 1}
    };

    constexpr int blinks = 75;

    for (int i = 0; i < blinks; ++i) {
        std::unordered_map<long, long> new_counts;

        for (const auto& [stone, count] : counts) {
            if (stone == 0) {
                new_counts[1] += count;
            } else {
                if (int digits = std::log10(stone) + 1; digits % 2 == 0) {
                    auto [left, right] = split(stone);
                    new_counts[left] += count;
                    new_counts[right] += count;
                } else {
                    new_counts[stone * 2024] += count;
                }
            }
        }
        counts = new_counts;
    }
    long total = 0;
    for (const auto& [stone, count] : counts) {
        total += count;
    }

    std::cout << total << std::endl;

    return 0;
}
