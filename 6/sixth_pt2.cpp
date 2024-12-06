#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

struct field {
    set<pair<int, int>> obstacles;
    pair<int, int> guard_pos;
    int cols = 0;
    int rows = 0;

    bool is_valid_position(const pair<int, int>& p) const {
        return p.first >= 0 && p.second >= 0 && p.first < rows && p.second < cols;
    }
};

set<pair<int, int>> run(const field& map) {
    auto curr_pos = map.guard_pos;
    pair dir = {0, -1};

    set<pair<int, int>> positions;
    positions.insert(curr_pos);

    while (map.is_valid_position(curr_pos)) {
        if (pair new_pos = {curr_pos.first + dir.first, curr_pos.second + dir.second}; map.obstacles.contains(new_pos)) {
            dir = {-dir.second, dir.first};
        } else {
            positions.insert(curr_pos = new_pos);
        }
    }
    positions.erase(curr_pos);
    return positions;
}

int main() {
    field f;
    ifstream fs("../6/map.txt");
    string line;
    while (getline(fs, line)) {
        for (int x = 0; x < line.size(); ++x) {
            if (line[x] == '#') {
                f.obstacles.emplace(x, f.rows);
            } else if (line[x] == '^') {
                f.guard_pos = {x, f.rows};
            }
        }
        f.cols = max(f.cols, static_cast<int>(line.size()));
        f.rows++;
    }

    auto visited = run(f);
    visited.erase(f.guard_pos);

    int loops = 0;

    for (const auto& pos : visited) {
        field new_map = f;
        new_map.obstacles.insert(pos);

        auto curr_pos = f.guard_pos;
        pair dir = {0, -1};

        set<pair<pair<int, int>, pair<int, int>>> seen_obstacles;

        while (new_map.is_valid_position(curr_pos)) {
            if (pair<int, int> new_pos = {curr_pos.first + dir.first, curr_pos.second + dir.second}; new_map.obstacles.contains(new_pos)) {
                auto state = make_pair(new_pos, dir);
                if (seen_obstacles.contains(state)) {
                    loops++;
                    break;
                }
                seen_obstacles.insert(state);
                dir = {-dir.second, dir.first};
            } else {
                curr_pos = new_pos;
            }
        }
    }

    cout << loops;
}