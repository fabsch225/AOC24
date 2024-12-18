//
// Created by fabian on 12/19/24.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <string>
#include <sstream>
#include <algorithm>
#include <optional>

using namespace std;

typedef pair<int, int> Point;

optional<size_t> bfs(vector<bool>& grid, size_t max_bytes, const vector<Point>& bytes) {
    fill(grid.begin(), grid.end(), false);

    for (size_t i = 0; i < max_bytes; ++i) {
        grid[bytes[i].second * 71 + bytes[i].first] = true;
    }

    tuple<int, int, int> pos = {0, 0, 0};
    deque<tuple<int, int, int>> queue;
    queue.push_back(pos);
    grid[0] = true;

    while (!queue.empty()) {
        auto [x, y, steps] = queue.front();
        queue.pop_front();

        if (x == 70 && y == 70) {
            return steps;
        }

        for (const auto& [dx, dy] : vector<Point>{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}) {
            int nx = x + dx;
            int ny = y + dy;
            size_t si = ny * 71 + nx;
            if (nx >= 0 && ny >= 0 && nx < 71 && ny < 71 && !grid[si]) {
                grid[si] = true;
                queue.push_back({nx, ny, steps + 1});
            }
        }
    }

    return nullopt;
}

vector<Point> parseInput(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not read file");
    }

    vector<Point> bytes;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string a, b;
        if (getline(ss, a, ',') && getline(ss, b)) {
            bytes.emplace_back(stoi(a), stoi(b));
        }
    }

    return bytes;
}

int main() {
    auto bytes = parseInput("../18/input.txt");

    vector<bool> grid(71 * 71, false);

    size_t total = bfs(grid, 1024, bytes).value_or(0);
    cout << total << endl;

    size_t min = 1025;
    size_t max = bytes.size();
    while (min != max) {
        size_t mid = (min + max) / 2;
        if (bfs(grid, mid, bytes).has_value()) {
            min = mid + 1;
        } else {
            max = mid;
        }
    }

    cout << bytes[max - 1].first << "," << bytes[max - 1].second << endl;

    return 0;
}
