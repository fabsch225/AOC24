#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

bool canProduceTarget(const vector<long long int>& nums, long long int target, long long int index, long long int currentValue) {
    if (index == nums.size()) {
        return currentValue == target;
    }

    if (canProduceTarget(nums, target, index + 1, currentValue + nums[index])) {
        return true;
    }
    
    if (canProduceTarget(nums, target, index + 1, currentValue * nums[index])) {
        return true;
    }

    return false;
}

long long int concat(long long int a, long long int b) {
    long long int numDigits = 0;
    long long int temp = b;
    while (temp > 0) {
        temp /= 10;
        numDigits++;
    }
    return a * static_cast<long long int>(pow(10, numDigits)) + b;
}

bool canProduceTargetWithConcatination(const vector<long long int>& nums, long long int target, long long int index, long long int currentValue) {
    if (currentValue > target) return false;
    if (index >= nums.size()) return currentValue == target;

    if (canProduceTargetWithConcatination(nums, target, index + 1, currentValue + nums[index])) {
        return true;
    }

    if (canProduceTargetWithConcatination(nums, target, index + 1, currentValue * nums[index])) {
        return true;
    }

    auto nums_concat_current = nums;
    nums_concat_current.erase(nums_concat_current.begin() + index);
    long long int concat_current = concat(currentValue, nums[index]);
    nums_concat_current.insert(nums_concat_current.begin() + index - 1, concat_current);
    if (canProduceTargetWithConcatination(nums_concat_current, target, index + 1, concat_current)) {
        return true;
    }

    return false;
}

int main() {
    string filepath = "../7/equations.txt";
    ifstream inputFile(filepath);

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file " << filepath << endl;
        return 1;
    }

    vector<pair<long long int, vector<long long int>>> equations;
    string line;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        long long int target;
        char colon;
        ss >> target >> colon;

        vector<long long int> nums;
        long long int num;
        while (ss >> num) {
            nums.push_back(num);
        }

        equations.push_back({target, nums});
    }

    inputFile.close();

    long long int totalCalibrationResult = 0;

    for (const auto&[fst, snd] : equations) {
        long long int target = fst;
        if (const vector<long long int>& nums = snd; canProduceTarget(nums, target, 1, nums[0])) {
            totalCalibrationResult += target;
        }
    }

    cout << "Part One: " << totalCalibrationResult << endl;

    long long int totalCalibrationResultWithConcatination = 0;

    for (const auto&[fst, snd] : equations) {
        long long int target = fst;

        if (const vector<long long int>& nums = snd; canProduceTargetWithConcatination(nums, target, 1, nums[0])) {
            totalCalibrationResultWithConcatination += target;
        }
    }

    cout << "Part Two: " << totalCalibrationResultWithConcatination << endl;

    return 0;
}
