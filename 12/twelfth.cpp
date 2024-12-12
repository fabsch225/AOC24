//
// Created by fschuller on 12/12/2024.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <queue>

//idea: breadth search.
//do until there are no more fields unvisited
//  do until there are no more fields of current type the same unvisited
//      if there are unvisited neighbours of the same type -> add them to the current queue
//      if there are invalid (edge) fields or fields of other type: -> count them
//  now, we should have counted the current region. multiply with the fence count


using namespace std;

constexpr int dx[] = {-1, 1, 0, 0};
constexpr int dy[] = {0, 0, -1, 1};

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
    int cost = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!visited[i][j]) {
                char type = map[i][j];
                int count = 0;
                int size = 0;

                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;

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
                            } else if (map[nx][ny] != type) {
                                count++;
                            }
                        } else {
                            count++;
                        }
                    }
                }
                cost += size * count;
            }
        }
    }

    cout << "Total cost of fences: " << cost << endl;

    return 0;
}
