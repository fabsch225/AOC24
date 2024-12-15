//
// Created by fabian on 12/13/24.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

pair<long long, long long> solve(pair<long long, long long> a1, pair<long long, long long> a2, pair<long long, long long> target) {
    long long x_a = a1.first, y_a = a1.second;
    long long x_b = a2.first, y_b = a2.second;
    long long x_t = target.first, y_t = target.second;
    double det = x_a * y_b - x_b * y_a;
    double det_x = x_t * y_b - y_t * x_b;
    double det_y = x_a * y_t - y_a * x_t;

    if (det_x == 0 || det_y == 0) return make_pair(0, 0);

    return make_pair(det_x / det, det_y / det);
}

pair<long long, long long> parse(const string &str, long long int correction) {
    string temp;
    string temp2;
    bool found_temp = false;
    for (unsigned long long i=0; i < str.size(); i++) {
        if (isdigit(str[i])) {
            if (found_temp) {
                while (isdigit(str[i])) {
                    temp2 += str[i];
                    i++;
                }
                break;
            }
            while (isdigit(str[i])) {
                temp += str[i];
                i++;
            }
            found_temp = true;
        }
    }
    return make_pair(stoi(temp) + correction, stoi(temp2) + correction);
}

int main() {
    vector<pair<long long, long long>> as;
    vector<pair<long long, long long>> bs;
    vector<pair<long long, long long>> targets;

    ifstream inputFile("../13/data13.txt");
    string line;
    int type = 0;
    while (getline(inputFile, line)) {
        if (type == 0) {
            as.push_back(parse(line, 0));
        } else if (type == 1) {
            bs.push_back(parse(line, 0));
        } else if (type == 2) {
            targets.push_back(parse(line, 10000000000000));
        }
        type = (type + 1) % 4;
    }

    long long tokens = 0;

    for (unsigned long i=0; i < as.size(); i++) {
        auto [fst, snd] = solve(as[i], bs[i], targets[i]);
        if (as[i].first * fst + bs[i].first * snd != targets[i].first || as[i].second * fst + bs[i].second * snd != targets[i].second) continue;
        cout << as[i].first << " * " << fst << " + " << bs[i].first << " * " << snd << " = " << targets[i].first << endl;
        cout << as[i].second << " * " << fst << " + " << bs[i].second << " * " << snd << " = " << targets[i].second << endl;
        tokens += fst * 3 + snd ;
    }

    cout << tokens;
}