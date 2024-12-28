//
// Created by fabian on 12/25/24.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::ifstream file("../25/input.txt");

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string input = buffer.str();

    std::vector<std::string> grids;
    size_t pos = 0;
    while ((pos = input.find("\n\n")) != std::string::npos) {
        grids.push_back(input.substr(0, pos));
        input.erase(0, pos + 2);
    }
    grids.push_back(input);

    std::vector<std::vector<size_t>> locks;
    std::vector<std::vector<size_t>> keys;

    size_t total_height = 0;

    for (const auto& g_str : grids) {
        std::vector<std::vector<char>> g;
        std::istringstream grid_stream(g_str);
        std::string line;

        while (std::getline(grid_stream, line)) {
            g.emplace_back(line.begin(), line.end());
        }

        bool is_key = g[0][0] != '#';
        total_height = g.size();

        std::vector<size_t> heights(g[0].size(), 0);
        for (const auto& row : g) {
            for (size_t x = 0; x < row.size(); ++x) {
                if (row[x] == '#') {
                    heights[x]++;
                }
            }
        }

        if (is_key) {
            keys.push_back(heights);
        } else {
            locks.push_back(heights);
        }
    }

    // check which keys fit into which locks
    size_t total = 0;
    for (const auto& k : keys) {
        for (const auto& l : locks) {
            if (std::equal(k.begin(), k.end(), l.begin(), [total_height](const size_t a, const size_t b) { return a + b <= total_height; })) {
                total++;
            }
        }
    }

    std::cout << total << std::endl;

    return 0;
}
