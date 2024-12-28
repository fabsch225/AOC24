//
// Created by fabian on 12/3/24.
//

#include <fstream>
#include <iostream>
#include <string>
#include <regex>

int main() {
    std::regex const e{R"(mul\((\d\d\d|\d\d|\d),(\d\d\d|\d\d|\d)\)|do\(\)|don't\(\))"};
    std::string fileName = "../3/program_pia.txt";
    std::ifstream inputFile(fileName);

    if (!inputFile) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return 1;
    }

    std::istreambuf_iterator<char> start(inputFile), end;
    std::string fileContent(start, end);

    auto it = std::sregex_iterator(fileContent.begin(), fileContent.end(), e);
    auto regexEnd = std::sregex_iterator();

    int result = 0;
    bool is_doing = true;
    for (; it != regexEnd; ++it) {
        std::smatch match = *it;
        std::string token = match.str();
        if (token == "do()") {
            is_doing = true;
            continue;
        }
        if (token == "don't()") {
            is_doing = false;
            continue;
        }
        if (!is_doing) continue;
        token = token.substr(4, token.size() - 5);
        size_t commaPos = token.find(',');
        int num1 = std::stoi(token.substr(0, commaPos));
        int num2 = std::stoi(token.substr(commaPos + 1));
        result += num1 * num2;
    }

    std::cout << result << std::endl;

    return 0;
}
