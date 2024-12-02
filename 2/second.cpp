//
// Created by fabian on 12/2/24.
//

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main() {
    std::string fileName = "../2/reports.txt";
    std::ifstream inputFile(fileName);

    if (!inputFile) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return 1;
    }

    int safe_count = 0;

    std::string line;
    int last_number = 0;
    int number;
    while (std::getline(inputFile, line)) {
        bool first_iteration = true;
        bool second_iteration = true;
        bool is_safe = true;
        bool is_ascending = false;
        size_t pos_start = 0, pos_end;
        line += ' ';
        while ((pos_end = line.find(' ', pos_start)) != std::string::npos) {
            number = stoi(line.substr(pos_start, pos_end - pos_start));
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
        if (is_safe) {
            safe_count++;
        }
    }

    cout << "Number of Safe reports: " << safe_count << endl;

    return 0;
}
