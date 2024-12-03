//
// Created by fabian on 12/3/24.
//

#include <fstream>
#include <iostream>
#include <string>
#include <regex>

//mul\((\d\d\d|\d\d|\d),(\d\d\d|\d\d|\d)\)|do\(\)|don't\(\)

int main() {
    std::regex const e{R"(mul\((\d\d\d|\d\d|\d),(\d\d\d|\d\d|\d)\))"};
    std::string fileName = "../3/program.txt";
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
    bool is_dooing = true;
    for (; it != regexEnd; ++it) {
        std::smatch match = *it;
        std::string cut = match.str();
        cut = cut.substr(4, cut.size() - 5);
        size_t commaPos = cut.find(',');
        int num1 = std::stoi(cut.substr(0, commaPos));
        int num2 = std::stoi(cut.substr(commaPos + 1));
        result += num1 * num2;
    }

    std::cout << result << std::endl;

    return 0;
}
