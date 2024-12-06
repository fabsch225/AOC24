//
// Created by fabian on 12/6/24.
//

#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

bool is_valid_position(const int x, const int y, const int rows, const int cols) {
    return x >= 0 && y >= 0 && x < rows && y < cols;
}

constexpr int dx[] = {1, 0, -1, 0};
constexpr int dy[] = {0, 1, 0, -1};

int turnRight(const int dir) {
    return (dir + 1) % 4;
}

int main() {
    vector<string> grid;
    string line;
    ifstream inputFile("../6/map.txt");

    int y, x;
    bool found = false;

    while (getline(inputFile, line)) {
        grid.push_back(line);
        if (int x_ = line.find_first_of('^'); x_ != string::npos) {
            x = x_;
            found = true;
        }
        if (!found) {
            y++;
        }
    }
    inputFile.close();

    const int rows = grid.size();
    const int cols = grid[0].size();
    int dir = 3;
    set<pair<int, int>> positions;
    positions.insert(make_pair(y, x));
    while (true) {
        if (!is_valid_position(y + dy[dir], x + dx[dir], rows, cols)) {
            positions.insert(make_pair(y, x));
            break;
        }
        if (grid[y + dy[dir]][x + dx[dir]] == '#') {
            dir = turnRight(dir);
        };
        y = y + dy[dir];
        x = x + dx[dir];
        positions.insert(make_pair(y, x));
    }

    cout << positions.size() << endl;
    return 0;
}
