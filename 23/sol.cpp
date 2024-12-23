//
// Created by fabia on 23.12.2024.
//

//https://www.geeksforgeeks.org/cliques-in-graph/
//TODO optimize

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>

int encode(const std::string& node) {
    return (node[0] - 'a') * 26 + (node[1] - 'a');
}

std::string decode(int node) {
    return std::string(1, 'a' + node / 26) + std::string(1, 'a' + node % 26);
}

int main() {
    std::ifstream input("../23/input.txt");

    std::vector<std::vector<int>> neighbors(26 * 26);
    std::vector edges(26 * 26, std::vector<bool>(26 * 26, false));

    std::string line;
    while (std::getline(input, line)) {
        auto pos = line.find('-');
        std::string a = line.substr(0, pos);
        std::string b = line.substr(pos + 1);
        int ai = encode(a);
        int bi = encode(b);
        edges[ai][bi] = true;
        edges[bi][ai] = true;
        neighbors[ai].push_back(bi);
        neighbors[bi].push_back(ai);
    }

    int total1 = 0;
    std::vector<int> largest_clique;

    for (int start = 0; start < 26 * 26; ++start) {
        if (neighbors[start].empty()) {
            continue;
        }

        std::queue<std::pair<int, std::vector<int>>> queue;
        queue.push({start, {start}});

        while (!queue.empty()) {
            int computer = queue.front().first;
            std::vector<int> seen = queue.front().second;
            queue.pop();

            for (int n : neighbors[computer]) {
                if (n < computer) {
                    continue;
                }

                bool valid = true;
                for (int s : seen) {
                    if (!edges[n][s]) {
                        valid = false;
                        break;
                    }
                }

                if (!valid) {
                    continue;
                }

                std::vector<int> new_seen = seen;
                new_seen.push_back(n);

                // 1
                if (new_seen.size() == 3 && (new_seen[0] / 26 == 19 || new_seen[1] / 26 == 19 || new_seen[2] / 26 == 19)) {
                    total1++;
                }

                // 2
                if (new_seen.size() > largest_clique.size()) {
                    largest_clique = new_seen;
                }

                queue.emplace(n, new_seen);
            }
        }
    }

    std::cout << total1 << std::endl;

    std::cout << "Largest Clique: ";
    for (int c : largest_clique) {
        std::cout << decode(c) << ",";
    }
    std::cout << std::endl;

    return 0;
}