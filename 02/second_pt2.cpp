//
// Created by fabian on 12/2/24.
//

//not really fast, but the thing is really try-and-errorly

//Thoughts about optimization:
//calculate the vector of numbers one time
//during the first check of validity, remember potential indices to be removed
//try out only these afterwards

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

bool is_safe(std::string line) {
    bool first_iteration = true;
    bool second_iteration = true;
    bool is_safe = true;
    bool is_ascending = false;
    int last_number = 0;
    size_t pos_start = 0, pos_end;
    line += ' ';
    while ((pos_end = line.find(' ', pos_start)) != std::string::npos) {
        const int number = stoi(line.substr(pos_start, pos_end - pos_start));
        pos_start = pos_end + 1;
        if (!first_iteration) {
            if (second_iteration) {
                if (last_number == number) {
                    is_safe = false;
                    break;
                }
                is_ascending = last_number < number;
                second_iteration = false;
            }
            if (std::abs(last_number - number) < 1 || std::abs(last_number - number) > 3) {
                is_safe = false;
                break;
            }
            if ((is_ascending && last_number > number) || (!is_ascending && last_number < number)) {
                is_safe = false;
                break;
            }
        } else {
            first_iteration = false;
        }
        last_number = number;
    }
    return is_safe;
}

vector<string> get_alternatives(const string& line_) {
    vector<string> alternatives;
    vector<int> numbers;
    vector<int> indices;
    string line = line_ + ' ';
    size_t pos_start = 0, pos_end;
    int index = 0;
    while ((pos_end = line.find(' ', pos_start)) != std::string::npos) {
        const int number = stoi(line.substr(pos_start, pos_end - pos_start));
        pos_start = pos_end + 1;
        numbers.push_back(number);
        indices.push_back(index);
        index++;
    }

    for (const auto index_to_be_removed: indices) {
        string alternative_line;
        for (int i = 0; i < indices.size(); i++) {
            if (i != index_to_be_removed) {
                alternative_line += to_string(numbers[i]) + ' ';
            }
        }
        alternatives.push_back(alternative_line.substr(0, alternative_line.size() - 1));
    }
    return alternatives;
}

int main() {
    std::string fileName = "../2/reports.txt";
    std::ifstream inputFile(fileName);

    if (!inputFile) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return 1;
    }

    int safe_count = 0;

    std::string line;
    while (std::getline(inputFile, line)) {
        if (is_safe(line)) {
            safe_count++;
        } else {
            auto alts = get_alternatives(line);
            for (const auto &alt: alts) {
                if (is_safe(alt)) {
                    safe_count++;
                    break;
                }
            }
        }
    }

    cout << "Number of Safe reports: " << safe_count << endl;

    return 0;
}
