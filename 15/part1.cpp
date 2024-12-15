//
// Created by fabian on 12/15/24.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cassert>

using namespace std;

class Grid {
    vector<vector<char>> grid;
    pair<int, int> curLoc;
    string moves;
    int rows, cols;

    map<char, pair<int, int>> symToDir = {
        {'>', {0, 1}},
        {'<', {0, -1}},
        {'^', {-1, 0}},
        {'v', {1, 0}}
    };

public:
    Grid(const string& filename) {
        parseInput(filename);
        findCurrentLocation();
    }

    void parseInput(const string& filename) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cerr << "Failed to open input file!" << endl;
            exit(1);
        }

        string line;
        bool readingMoves = false;

        while (getline(inputFile, line)) {
            if (line.empty()) {
                readingMoves = true;
                continue;
            }

            if (readingMoves) {
                moves += line;
            } else {
                grid.push_back(vector<char>(line.begin(), line.end()));
            }
        }
        inputFile.close();

        rows = grid.size();
        cols = grid[0].size();
    }

    void findCurrentLocation() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == '@') {
                    curLoc = {i, j};
                    return;
                }
            }
        }
    }

    void executeMoves() {
        for (char move : moves) {
            auto dir = symToDir[move];
            const pair<int, int> newLoc = {curLoc.first + dir.first, curLoc.second + dir.second};
            pair<int, int> finalLoc = newLoc;

            while (grid[finalLoc.first][finalLoc.second] != '.' &&
                   grid[finalLoc.first][finalLoc.second] != '#') {
                finalLoc.first += dir.first;
                finalLoc.second += dir.second;
            }

            if (grid[finalLoc.first][finalLoc.second] != '#') {
                assert(grid[newLoc.first][newLoc.second] != '#');

                grid[finalLoc.first][finalLoc.second] = grid[newLoc.first][newLoc.second];
                grid[newLoc.first][newLoc.second] = '@';
                grid[curLoc.first][curLoc.second] = '.';
                curLoc = newLoc;
            }
        }
    }

    int calculateAnswer() {
        int ans = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 'O') {
                    ans += 100 * i + j;
                }
            }
        }
        return ans;
    }

    void printGrid() const {
        for (const auto& row : grid) {
            for (char cell : row) {
                cout << cell;
            }
            cout << endl;
        }
    }
};

int main() {
    Grid game("../15/input.txt");

    game.executeMoves();

    cout << "Final Grid:" << endl;
    game.printGrid();


    cout << "\nAnswer: " << game.calculateAnswer();;

    return 0;
}
