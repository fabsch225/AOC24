//
// Created by fabian on 12/4/24.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

bool isValid(int x, int y, int rows, int cols) {
    return x >= 1 && x < rows-1 && y >= 1 && y < cols-1;
}

int countXMASAt(const vector<string>& grid, const int x, const int y) {
    if (grid[x - 1][y - 1] == 'M' && grid[x + 1][y + 1] == 'S') {
        if (grid[x - 1][y + 1] == 'M' && grid[x + 1][y - 1] == 'S') {
            return 1;
        }
        if (grid[x - 1][y + 1] == 'S' && grid[x + 1][y - 1] == 'M') {
            return 1;
        }
    }
    if (grid[x + 1][y + 1] == 'M' && grid[x - 1][y - 1] == 'S') {
        if (grid[x - 1][y + 1] == 'M' && grid[x + 1][y - 1] == 'S') {
            return 1;
        };
        if (grid[x - 1][y + 1] == 'S' && grid[x + 1][y - 1] == 'M') {
            return 1;
        };
    };

    return 0;
}

int countXMAS(const vector<string>& grid) {
    const int rows = grid.size();
    const int cols = grid[0].size();
    int totalCount = 0;

    for (int x = 1; x < rows - 1; x++) {
        for (int y = 1; y < cols - 1; y++) {
            if (grid[x][y] == 'A' && isValid(x, y, rows, cols)) {
                totalCount += countXMASAt(grid, x, y);
            }
        }
    }

    return totalCount;
}

int main() {
    vector<string> grid;
    string line;
    ifstream inputFile("../4/grid.txt");

    while (getline(inputFile, line)) {
        grid.push_back(line);
    }
    inputFile.close();

    int result = countXMAS(grid);
    cout << "The X-MAS pattern appears " << result << " times in the grid." << endl;

    return 0;
}
