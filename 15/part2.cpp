//
// Created by fabian on 12/15/24.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cassert>

using namespace std;

class Grid {
    vector<vector<char>> origGrid;
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

    set<pair<int, int>> affecting(int x, int y) {
        set<pair<int, int>> result;
        if (grid[x][y] == '.')
            return result;
        if (grid[x][y] == '[')
            return {{x, y}, {x, y + 1}};
        if (grid[x][y] == ']')
            return {{x, y}, {x, y - 1}};
        return result;
    }

public:
    Grid(const string& filename) {
        parseInput(filename);
        expandGrid();
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
                vector<char> row(line.begin(), line.end());
                origGrid.push_back(row);
            }
        }
        inputFile.close();
    }

    void expandGrid() {
        rows = origGrid.size();
        cols = origGrid[0].size();
        grid = vector<vector<char>>(rows, vector<char>());

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (origGrid[i][j] == '#') {
                    grid[i].push_back('#');
                    grid[i].push_back('#');
                } else if (origGrid[i][j] == 'O') {
                    grid[i].push_back('[');
                    grid[i].push_back(']');
                } else if (origGrid[i][j] == '.') {
                    grid[i].push_back('.');
                    grid[i].push_back('.');
                } else if (origGrid[i][j] == '@') {
                    curLoc = {i, (int)grid[i].size()};
                    grid[i].push_back('@');
                    grid[i].push_back('.');
                }
            }
        }

        rows = grid.size();
        cols = grid[0].size();
    }

    void executeMoves() {
        for (char move : moves) {
            auto dir = symToDir[move];
            pair<int, int> newLoc = {curLoc.first + dir.first, curLoc.second + dir.second};

            if (dir.first == 0 || grid[newLoc.first][newLoc.second] == '.') {
                processEmptyMove(newLoc, dir);
            } else if (grid[newLoc.first][newLoc.second] != '#') {
                processPushMove(newLoc, dir);
            }
        }
    }

    void processEmptyMove(pair<int, int>& newLoc, pair<int, int>& dir) {
        pair<int, int> finalLoc = newLoc;
        while (grid[finalLoc.first][finalLoc.second] != '.' &&
               grid[finalLoc.first][finalLoc.second] != '#') {
            finalLoc.first += dir.first;
            finalLoc.second += dir.second;
        }

        if (grid[finalLoc.first][finalLoc.second] != '#') {
            while ((finalLoc.first - dir.first != curLoc.first) ||
                   (finalLoc.second - dir.second != curLoc.second)) {
                grid[finalLoc.first][finalLoc.second] =
                    grid[finalLoc.first - dir.first][finalLoc.second - dir.second];
                finalLoc.first -= dir.first;
                finalLoc.second -= dir.second;
            }

            grid[newLoc.first][newLoc.second] = '@';
            grid[curLoc.first][curLoc.second] = '.';
            curLoc = newLoc;
        }
    }

    void processPushMove(pair<int, int>& newLoc, pair<int, int>& dir) {
        vector<set<pair<int, int>>> affectings = {affecting(newLoc.first, newLoc.second)};
        bool blocked = false;

        while (!affectings.back().empty()) {
            set<pair<int, int>> newAffect;

            for (auto& [x, y] : affectings.back()) {
                if (grid[x + dir.first][y] == '#') {
                    blocked = true;
                    break;
                }
                for (auto& [xd, yd] : affecting(x + dir.first, y)) {
                    newAffect.insert({xd, yd});
                }
            }

            if (blocked)
                break;

            affectings.push_back(newAffect);
        }

        if (!blocked) {
            for (int i = affectings.size() - 1; i >= 0; --i) {
                for (auto& [x, y] : affectings[i]) {
                    grid[x + dir.first][y] = grid[x][y];
                    grid[x][y] = '.';
                }
            }

            grid[newLoc.first][newLoc.second] = '@';
            grid[curLoc.first][curLoc.second] = '.';
            curLoc = newLoc;
        }
    }

    void printGrid() {
        for (const auto& row : grid) {
            for (char cell : row) {
                cout << cell;
            }
            cout << endl;
        }
    }

    int calculateAnswer() {
        int ans = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == '[') {
                    ans += 100 * i + j;
                }
            }
        }
        return ans;
    }
};

int main() {
    Grid game("../15/input.txt");

    cout << "Initial Grid:" << endl;
    game.printGrid();

    game.executeMoves();

    cout << "Final Grid:" << endl;
    game.printGrid();

    int answer = game.calculateAnswer();
    cout << "Answer: " << answer << endl;

    return 0;
}
