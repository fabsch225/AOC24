//
// Created by fabian on 12/10/24.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <set>

using namespace std;

typedef pair<int, int> pos_t;

pos_t add_positions(const pos_t& a, const pos_t& b) {
    return make_pair(a.first + b.first, a.second + b.second);
}

constexpr pos_t directions[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

class grid_t {
public:
    vector<string> data;
    int rows;
    int cols;

    bool is_valid(const pos_t &pos) const {
        return pos.first >= 0 && pos.second >= 0 && pos.first < rows && pos.second < cols;
    }

    void init() {
        rows = data.size();
        cols = data[0].size();
    }
};


void collect_nines(const pos_t currentPos, const grid_t &grid, set<pos_t> &nines) {
    if (grid.data[currentPos.first][currentPos.second] == '9') {
        nines.insert(currentPos);
        return;
    }
    for (int i = 0; i < 4; i++) {
        auto new_pos = add_positions(currentPos, directions[i]);
        if (!grid.is_valid(new_pos)) { continue; }
        if (grid.data[new_pos.first][new_pos.second] - grid.data[currentPos.first][currentPos.second] == 1) {
            collect_nines(new_pos, grid, nines);
        }
    }
}

int count_distinct_trails(const pos_t currentPos, const grid_t &grid) {
    if (grid.data[currentPos.first][currentPos.second] == '9') {
        return 1;
    }
    int count = 0;
    for (int i = 0; i < 4; i++) {
        auto new_pos = add_positions(currentPos, directions[i]);
        if (!grid.is_valid(new_pos)) { continue; }
        if (grid.data[new_pos.first][new_pos.second] - grid.data[currentPos.first][currentPos.second] == 1) {
            count += count_distinct_trails(new_pos, grid);
        }
    }
    return count;
}

int main() {
    grid_t grid;
    string line;
    ifstream inputFile("../10/map10.txt");
    vector<pos_t> start_positions;
    int row = 0;
    while (getline(inputFile, line)) {
        grid.data.push_back(line);
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == '0') {
                start_positions.emplace_back(pos_t{row, i});
            }
        }
        row++;
    }
    inputFile.close();
    grid.init();

    int count = 0;
    for (auto p: start_positions) {
        set<pos_t> nines;
        collect_nines(p, grid, nines);
        count += nines.size();
    }

    cout << "Total Score: " << count << endl;

    int count2 = 0;
    for (auto p: start_positions) {
        count2 += count_distinct_trails(p, grid);
    }

    cout << "Total Score: " << count2 << endl;

    return 0;
}
