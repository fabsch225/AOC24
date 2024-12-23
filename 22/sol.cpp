//
// Created by fabia on 23.12.2024.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

int main() {
    std::ifstream file("../22/input.txt");

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    long long total1 = 0;
    int total2 = 0;
    constexpr int MASK = (1 << 20) - 1;

    std::vector<int> bananas(1 << 20, 0);
    std::vector<size_t> seen(1 << 20, std::numeric_limits<size_t>::max());

    for (size_t j = 0; j < lines.size(); ++j) {
        long long n = std::stoll(lines[j]);

        size_t current_sequence = 0;
        int old_price = n % 10;

        for (int i = 0; i < 2000; ++i) {
            n ^= n << 6;
            n &= 16777215;

            n ^= n >> 5;

            n ^= n << 11;
            n &= 16777215;

            const int price = n % 10;
            const int diff = price - old_price;

            current_sequence = ((current_sequence << 5) & MASK) + (diff + 10);

            if (i >= 3 && seen[current_sequence] != j) {
                seen[current_sequence] = j;
                bananas[current_sequence] += price;
                total2 = std::max(total2, bananas[current_sequence]);
            }

            old_price = price;
        }

        total1 += n;
    }

    std::cout << total1 << std::endl;
    std::cout << total2 << std::endl;

    return 0;
}
