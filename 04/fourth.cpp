//
// Created by fabian on 12/4/24.
//
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

const int dx[] = {0, 0, 1, -1, 1, 1, -1, -1};
const int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};

bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

bool findWord(const vector<string>& grid, int x, int y, int dir) {
    const string word = "XMAS";
    int rows = grid.size();
    int cols = grid[0].size();
    for (int k = 0; k < word.size(); k++) {
        int nx = x + k * dx[dir];
        int ny = y + k * dy[dir];
        if (!isValid(nx, ny, rows, cols) || grid[nx][ny] != word[k]) {
            return false;
        }
    }
    return true;
}

int countOccurrences(const vector<string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    int count = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int dir = 0; dir < 8; dir++) { // Check all 8 directions
                if (findWord(grid, i, j, dir)) {
                    count++;
                }
            }
        }
    }

    return count;
}

int main() {
    vector<string> grid;
    string line;
    ifstream inputFile("../4/grid.txt");

    while (getline(inputFile, line)) {
        grid.push_back(line);
    }
    inputFile.close();

    int result = countOccurrences(grid);
    cout << "The word XMAS appears " << result << " times in the grid." << endl;

    return 0;
}
