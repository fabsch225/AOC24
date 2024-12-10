//
// Created by fabian on 12/9/24.
//

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

typedef pair<int, int> pos_t;

class map_t {
public:
    vector<string> grid;
    vector<vector<pos_t>> nodes;
    set<pos_t> all_nodes;
    int rows, cols;

    bool is_valid(const pos_t &node) const {
        return node.first >= 0 && node.second >= 0 && node.first < rows && node.second < cols;// && !all_nodes.contains(node);
    }

    void init() {
        rows = grid.size();
        cols = grid[0].size();
        map<char, int> frequencies;
        int unique_counter = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] != '.') {
                    if (auto index = frequencies.find(grid[i][j]); index != frequencies.end()) {
                        const auto pos = pos_t(i, j);
                        all_nodes.insert(pos);
                        nodes[index->second].push_back(pos);
                    }
                    else {
                        frequencies.insert(make_pair(grid[i][j], unique_counter));
                        nodes.push_back(vector<pos_t>());
                        const auto pos = pos_t(i, j);
                        all_nodes.insert(pos);
                        nodes[unique_counter].push_back(pos);
                        unique_counter++;
                    }
                }
            }
        }
    }
};

void add_linear_nodes(const pos_t &a, const pos_t &b, set<pos_t> &nodes, map_t &map) {
    const int dy = a.first - b.first;
    const int dx = a.second - b.second;

    for (int i = 0; i < map.cols; i++) {
        auto new_pos = pos_t(a.first + i * dy, a.second + i * dx);
        if (map.is_valid(new_pos)) {
            nodes.insert(new_pos);
        } else {
            break;
        }
    }
}


int main() {
    //load data
    //identify classes (frequency) of nodes
    //for each pair, calculate the viable points
    //store them in set to get uniqueness

    map_t map;
    string line;
    ifstream inputFile("../8/map.txt");
    while (getline(inputFile, line)) {
        map.grid.push_back(line);
    }
    inputFile.close();
    map.init();

    set<pos_t> affected_positions;

    for (auto freq : map.nodes) {
        for (int i = 0; i < freq.size(); i++) {
            for (int j = i; j < freq.size(); j++) {
                //calculate the relevant points for each pair (i, j)
                int dy = freq[i].first - freq[j].first;
                int dx = freq[i].second - freq[j].second;
                const auto pos_a = pos_t{freq[i].first + dy, freq[i].second + dx};
                const auto pos_b = pos_t{freq[j].first - dy, freq[j].second - dx};
                if (map.is_valid(pos_a) && dx != 0 && dy != 0) {
                    affected_positions.insert(pos_a);
                }
                if (map.is_valid(pos_b) && dx != 0 && dy != 0) {
                    affected_positions.insert(pos_b);
                }
            }
        }
    }

    cout << "Part 1: Affected Positions: " << affected_positions.size() << endl;

    set<pos_t> more_affected_positions;

    for (auto freq : map.nodes) {
        for (int i = 0; i < freq.size(); i++) {
            for (int j = i; j < freq.size(); j++) {
                add_linear_nodes(freq[i], freq[j], more_affected_positions, map);
                add_linear_nodes(freq[j], freq[i], more_affected_positions, map);
            }
        }
    }

    cout << "Part 2: Affected Positions: " << more_affected_positions.size() << endl;
}
