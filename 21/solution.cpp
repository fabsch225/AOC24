#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <fstream>
#include <memory>
#include <optional>

using namespace std;

const vector<pair<int, int>> DIRS = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

const vector<vector<char>> PAD = {
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {' ', '0', 'A'},
};

const vector<vector<char>> INPUTS = {
    {' ', '^', 'A'},
    {'<', 'v', '>'},
};

struct custom_hash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        const size_t h1 = hash<T1>()(p.first);
        const size_t h2 = hash<T2>()(p.second);
        return h1 ^ (h2 << 1);
    }
};

class KeypadSolver {
public:
    KeypadSolver() : path_cache(128 * 128, nullopt) {}

    shared_ptr<vector<vector<char>>> find_paths(
        const vector<vector<char>>& keypad,
        const char from,
        const char to
    ) {
        const int index = (from - 'A') * 128 + (to - 'A');
        if (path_cache[index].has_value()) {
            return path_cache[index].value();
        }

        if (from == to) {
            auto result = make_shared<vector<vector<char>>>();
            result->push_back({ 'A' });
            path_cache[index] = result;
            return result;
        }


        pair<int, int> start, end;
        for (int y = 0; y < keypad.size(); ++y) {
            for (int x = 0; x < keypad[y].size(); ++x) {
                if (keypad[y][x] == from) {
                    start = { x, y };
                }
                if (keypad[y][x] == to) {
                    end = { x, y };
                }
            }
        }

        vector dists(keypad.size(), vector(keypad[0].size(), LONG_LONG_MAX));
        queue<tuple<int, int, int>> q;
        q.emplace( start.first, start.second, 0 );

        while (!q.empty()) {
            auto [x, y, steps] = q.front();
            q.pop();
            dists[y][x] = steps;
            for (auto [dx, dy] : DIRS) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && ny >= 0 && nx < 3 && ny < keypad.size() && keypad[ny][nx] != ' ' && dists[ny][nx] == LONG_LONG_MAX) {
                    q.emplace( nx, ny, steps + 1 );
                }
            }
        }

        vector<vector<char>> paths;
        vector<tuple<int, int, vector<char>>> stack;
        stack.push_back({ end.first, end.second, { 'A' } });
        while (!stack.empty()) {
            auto [x, y, path] = stack.back();
            stack.pop_back();
            if (x == start.first && y == start.second) {
                paths.push_back(path);
                continue;
            }
            for (long long int i = 0; i < DIRS.size(); ++i) {
                auto [dx, dy] = DIRS[i];
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && ny >= 0 && nx < 3 && ny < keypad.size() && dists[ny][nx] < dists[y][x]) {
                    const char c = "<^>v"[i];
                    vector new_path = { c };
                    new_path.insert(new_path.end(), path.begin(), path.end());
                    stack.emplace_back( nx, ny, new_path );
                }
            }
        }

        auto result = make_shared<vector<vector<char>>>(paths);
        path_cache[index] = result;
        return result;
    }

    long long int find_sequence(
        const string& s,
        long long int depth,
        const bool highest
    ) {
        const pair cache_key = { s, depth };
        if (this->cache.contains(cache_key)) {
            return this->cache[cache_key];
        }

        char cursor = 'A';
        long long int result = 0;
        for (const char c : s) {
            auto paths = this->find_paths(highest ? PAD : INPUTS, cursor, c);
            if (depth == 0) {
                result += (*paths)[0].size();
            } else {
                long long int min_len = LONG_LONG_MAX;
                for (const auto& p : *paths) {
                    min_len = min(min_len, this->find_sequence(string(p.begin(), p.end()), depth - 1, false));

                }
                result += min_len;
            }
            cursor = c;
        }

        this->cache[cache_key] = result;
        return result;
    }

private:
    unordered_map<pair<string, long long int>, long long int, custom_hash> cache;
    vector<optional<shared_ptr<vector<vector<char>>>> > path_cache;
};

int main() {
    ifstream file("../21/input.txt");

    KeypadSolver solver;

    string line;

    const int max_depth = 25;
    long long int total = 0;

    while (getline(file, line)) {
        const int n = stoi(line.substr(0, 3));
        const long long int len = solver.find_sequence(line, max_depth, true);
        total += n * len;
    }

    cout << total << endl;

    return 0;
}