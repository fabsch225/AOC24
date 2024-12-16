#include <climits>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

using State = pair<int, int>;

struct StateHash {
    size_t operator()(const State &s) const {
        return hash<int>()(s.first) ^ (hash<int>()(s.second) << 1);
    }
};

struct Node {
    int score;
    int pos;
    int dir;
    string path;

    bool operator>(const Node &other) const {
        return score > other.score;
    }
};

int main() {
    ifstream file("../16/map.txt");

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    vector<char> maze;
    for (const auto &l : lines) {
        maze.insert(maze.end(), l.begin(), l.end());
    }
    int start = -1, end = -1;
    for (int i = 0; i < maze.size(); i++) {
        if (maze[i] == 'S') start = i;
        if (maze[i] == 'E') end = i;
    }

    int dimx = lines[0].size();
    vector<int> dirs = {-dimx, 1, dimx, -1};

    unordered_map<State, int, StateHash> visited;
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    int highscore = INT_MAX;

    vector<string> paths;

    pq.push({0, start, 1, ""});

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        int score = current.score;
        int pos = current.pos;
        int d = current.dir;
        string path = current.path;

        if (score > highscore) break;

        if (visited.find({pos, d}) != visited.end() && visited[{pos, d}] < score) {
            continue;
        }
        visited[{pos, d}] = score;

        if (pos == end) {
            highscore = score;
            paths.push_back(path);
        }

        if (maze[pos + dirs[d]] != '#') {
            pq.push({score + 1, pos + dirs[d], d, path + "F"});
        }
        pq.push({score + 1000, pos, (d + 1) % 4, path + "R"});
        pq.push({score + 1000, pos, (d - 1 + 4) % 4, path + "L"});
    }

    unordered_set<int> tiles;
    tiles.insert(start);

    for (const string &p : paths) {
        int t = start, d = 1;
        for (char c : p) {
            if (c == 'L') {
                d = (d - 1 + 4) % 4;
            } else if (c == 'R') {
                d = (d + 1) % 4;
            } else if (c == 'F') {
                t += dirs[d];
                tiles.insert(t);
            }
        }
    }

    cout << "Score: " << highscore << endl;
    cout << "pos Count: " << tiles.size() << endl;

    return 0;
}
