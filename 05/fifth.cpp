//
// Created by fabian on 12/5/24.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

void parseRules(const vector<string>& rulesInput, unordered_map<int, unordered_set<int>>& rules) {
    for (const auto& rule : rulesInput) {
        int x, y;
        char separator;
        stringstream ss(rule);
        ss >> x >> separator >> y;
        rules[x].insert(y);
    }
}

bool isCorrectOrder(const vector<int>& update, const unordered_map<int, unordered_set<int>>& rules) {
    unordered_map<int, int> pageIndices;
    for (int i = 0; i < update.size(); ++i) {
        pageIndices[update[i]] = i;
    }
    for (const auto& [x, ySet] : rules) {
        if (pageIndices.find(x) != pageIndices.end()) {
            const int xIndex = pageIndices[x];
            for (int y : ySet) {
                if (pageIndices.find(y) != pageIndices.end()) {
                    const int yIndex = pageIndices[y];
                    if (xIndex >= yIndex) return false;
                }
            }
        }
    }
    return true;
}

int getMiddlePage(const vector<int>& update) {
    const int n = update.size();
    return update[n / 2];
}

bool loadData(const string& filePath, vector<string>& rulesInput, vector<vector<int>>& updates) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return false;
    }

    string line;
    bool isRulesSection = true;

    while (getline(file, line)) {
        if (line.empty()) {
            isRulesSection = false;
            continue;
        }

        if (isRulesSection) {
            rulesInput.push_back(line);
        } else {
            vector<int> update;
            stringstream ss(line);
            int page;
            while (ss >> page) {
                update.push_back(page);
                if (ss.peek() == ',') ss.ignore();
            }
            updates.push_back(update);
        }
    }

    file.close();
    return true;
}

int main() {
    const string filePath = "../5/protocols.txt";

    vector<string> rulesInput;
    vector<vector<int>> updates;

    if (!loadData(filePath, rulesInput, updates)) {
        return 1;
    }

    unordered_map<int, unordered_set<int>> rules;
    parseRules(rulesInput, rules);

    int middlePageSum = 0;

    for (const auto& update : updates) {
        if (isCorrectOrder(update, rules)) {
            middlePageSum += getMiddlePage(update);
        }
    }

    cout << "Sum: " << middlePageSum << endl;

    return 0;
}