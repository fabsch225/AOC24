//
// Created by fschuller on 12/12/2024.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std;

constexpr int dx[] = {-1, 0, 1, 0};
constexpr int dy[] = {0, -1, 0, 1};

bool is_valid(const int x, const int y, const int rows, const int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

int main() {
    vector<string> map;

    ifstream inputFile("../12/map12.txt");
    string line;
    while (getline(inputFile, line)) {
        map.push_back(line);
    }

    const int rows = map.size();
    const int cols = map[0].size();

    vector visited(rows, vector<bool>(cols, false));
    vector visited_edges(rows, vector(cols, vector<bool>(4, false)));
    int cost = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!visited[i][j]) {
                char type = map[i][j];
                int count = 0;
                int size = 0;

                set<int> sides_x;
                set<int> sides_y;
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;

                auto sub_search_in_direction = [&](int x, int y, int move_dir, int orig_dir) {
                    int nx = x + dx[move_dir];
                    int ny = y + dy[move_dir];
                    int ox = nx + dx[orig_dir];
                    int oy = ny + dy[orig_dir];
                    while (is_valid(nx, ny, rows, cols) && map[nx][ny] == type && (!is_valid(ox, oy, rows, cols) || map[ox][oy] != type)) {
                        visited_edges[nx][ny][orig_dir] = true;
                        if (!visited[nx][ny]) {
                            //i imagine this improves performance very slightly
                            visited[nx][ny] = true;
                            q.push({nx, ny});
                        }
                        nx += dx[move_dir];
                        ny += dy[move_dir];
                        ox = nx + dx[orig_dir];
                        oy = ny + dy[orig_dir];
                    }
                };

                auto sub_search_current_edge = [&](int x, int y, int dir) {
                    int perp_1 = (dir + 1) % 4;
                    int perp_2 = (perp_1 + 2) % 4;
                    sub_search_in_direction(x, y, perp_1, dir);
                    sub_search_in_direction(x, y, perp_2, dir);
                };

                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    size++;

                    for (int d = 0; d < 4; ++d) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];

                        if (is_valid(nx, ny, rows, cols)) {
                            if (map[nx][ny] == type && !visited[nx][ny]) {
                                visited[nx][ny] = true;
                                q.push({nx, ny});
                            } else if (map[nx][ny] != type && !visited_edges[x][y][d]) {
                                count++;
                                sub_search_current_edge(x, y, d);
                            }
                        } else if (!visited_edges[x][y][d]) {
                            count++;
                            sub_search_current_edge(x, y, d);
                        }
                    }
                }
                cout << map[i][j] << ": "<< size << " * " << count << " = " << count * size << endl;

                cost += size * count;
            }
        }
    }

    cout << "Total cost of fences: " << cost << endl;

    return 0;
}
