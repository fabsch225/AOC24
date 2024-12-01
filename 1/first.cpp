//
// Created by fabian on 12/1/24.
//


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int calculateTotalDistance(const vector<int>& leftList, const vector<int>& rightList) {
    int totalDistance = 0;
    for (size_t i = 0; i < leftList.size(); i++) {
        totalDistance += abs(leftList[i] - rightList[i]);
    }
    return totalDistance;
}

int main() {
    string fileName = "../1/locations.txt";
    ifstream inputFile(fileName);

    if (!inputFile) {
        cerr << "Error: Could not open file " << fileName << endl;
        return 1;
    }

    vector<int> leftList;
    vector<int> rightList;
    int leftValue, rightValue;

    while (inputFile >> leftValue >> rightValue) {
        leftList.push_back(leftValue);
        rightList.push_back(rightValue);
    }

    inputFile.close();

    sort(leftList.begin(), leftList.end());
    sort(rightList.begin(), rightList.end());

    if (leftList.size() != rightList.size()) {
        cerr << "Error: The lists have different sizes." << endl;
        return 1;
    }

    int totalDistance = calculateTotalDistance(leftList, rightList);

    cout << "Total distance between the lists: " << totalDistance << endl;

    return 0;
}
