//
// Created by fabian on 12/1/24.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

int calculateSimilarityScore(const vector<int>& leftList, const vector<int>& rightList) {
    unordered_map<int, int> frequencyMap;

    for (int num : rightList) {
        frequencyMap[num]++;
    }

    int similarityScore = 0;

    for (int num : leftList) {
        similarityScore += num * frequencyMap[num];
    }

    return similarityScore;
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

    int similarityScore = calculateSimilarityScore(leftList, rightList);

    cout << "Similarity score between the lists: " << similarityScore << endl;

    return 0;
}
