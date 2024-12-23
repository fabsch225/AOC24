//
// Created by fabia on 23.12.2024.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

constexpr int DIRS[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

std::vector<std::pair<int, int>> find_non_branching_path(
    const std::vector<char>& grid,
    size_t width,
    std::pair<int, int> start,
    std::pair<int, int> end
) {
    int dir = 0;
    for (int i = 0; i < 4; ++i) {
        int nx = start.first + DIRS[i][0];
        int ny = start.second + DIRS[i][1];
        if (grid[ny * width + nx] != '#') {
            dir = i;
            break;
        }
    }

    std::pair<int, int> pos = start;
    std::vector<std::pair<int, int>> result = {start};
    while (pos != end) {
        int nx = pos.first + DIRS[dir][0];
        int ny = pos.second + DIRS[dir][1];

        if (grid[ny * width + nx] == '#') {
            int right = (dir + 1) % 4;
            if (grid[(pos.second + DIRS[right][1]) * width + (pos.first + DIRS[right][0])] != '#') {
                dir = right;
                continue;
            }

            int left = (dir + 3) % 4;
            if (grid[(pos.second + DIRS[left][1]) * width + (pos.first + DIRS[left][0])] != '#') {
                dir = left;
                continue;
            }

            throw std::runtime_error("Path must not branch");
        }

        pos = {nx, ny};
        result.push_back(pos);
    }

    return result;
}

int main() {
    std::ifstream input("../20/in.txt");
    if (!input.is_open()) {
        std::cerr << "Could not read file" << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    size_t width = lines[0].size();
    size_t height = lines.size();
    std::vector<char> grid;
    for (const auto& l : lines) {
        grid.insert(grid.end(), l.begin(), l.end());
    }

    std::pair<int, int> start, end;
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            char c = grid[y * width + x];
            if (c == 'S') {
                start = {static_cast<int>(x), static_cast<int>(y)};
            } else if (c == 'E') {
                end = {static_cast<int>(x), static_cast<int>(y)};
            }
        }
    }

    auto path = find_non_branching_path(grid, width, start, end);
    size_t max = path.size() - 1;

    int cell_size = 20;
    int cells_x = (width + cell_size - 1) / cell_size;
    int cells_y = (height + cell_size - 1) / cell_size;
    std::vector<std::vector<std::tuple<int, int, int>>> cells(cells_x * cells_y);

    for (size_t i = 0; i < path.size(); ++i) {
        auto [px, py] = path[i];
        cells[(py / cell_size) * cells_x + (px / cell_size)].emplace_back(i, px, py);
    }

    int total1 = 0, total2 = 0;
    for (int cy = 0; cy < cells_y; ++cy) {
        for (int cx = 0; cx < cells_x; ++cx) {
            int min_cx = std::max(0, cx - 1);
            int max_cx = std::min(cells_x - 1, cx + 1);
            int min_cy = std::max(0, cy - 1);
            int max_cy = std::min(cells_y - 1, cy + 1);

            while (!cells[cy * cells_x + cx].empty()) {
                auto [si, sx, sy] = cells[cy * cells_x + cx].back();
                cells[cy * cells_x + cx].pop_back();

                for (int cy2 = min_cy; cy2 <= max_cy; ++cy2) {
                    for (int cx2 = min_cx; cx2 <= max_cx; ++cx2) {
                        for (const auto& [ei, ex, ey] : cells[cy2 * cells_x + cx2]) {
                            int dist = std::abs(sx - ex) + std::abs(sy - ey);
                            if (int m = max - std::abs(ei - si) + dist; m >= max || max - m < 100) {
                                continue;
                            }

                            if (dist <= 2) {
                                total1 += 1;
                            }
                            if (dist <= 20) {
                                total2 += 1;
                            }
                        }
                    }
                }
            }
        }
    }

    std::cout << total1 << std::endl;
    std::cout << total2 << std::endl;

    return 0;
}
