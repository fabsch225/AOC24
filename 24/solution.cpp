//
// Created by fabian on 12/25/24.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>
#include <algorithm>
#include <cstdint>
#include <string>

enum class Logic {
    And,
    Or,
    Xor
};

struct Gate {
    Logic logic;
    std::string a;
    std::string b;
    std::string out;
};

uint64_t get_value(const std::unordered_map<std::string, bool>& wires, const std::string& prefix) {
    std::vector<std::pair<std::string, bool>> wires_to_decode;

    for (const auto& [name, value] : wires) {
        if (name.find(prefix) == 0) {
            wires_to_decode.emplace_back(name, value);
        }
    }

    std::ranges::sort(wires_to_decode, [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    std::ranges::reverse(wires_to_decode);

    uint64_t result = 0;
    for (const auto& [_, value] : wires_to_decode) {
        result <<= 1;
        if (value) {
            result += 1;
        }
    }

    return result;
}

uint64_t run(
    std::unordered_map<std::string, bool> wires,
    const std::vector<Gate>& gates,
    const std::unordered_map<std::string, std::string>& renames
) {
    std::deque queue(gates.begin(), gates.end());

    while (!queue.empty()) {
        Gate gate = queue.front();
        queue.pop_front();

        std::string out = renames.contains(gate.out) ? renames.at(gate.out) : gate.out;

        if (!wires.contains(gate.a) || !wires.contains(gate.b)) {
            queue.push_back(gate);
            continue;
        }

        bool a = wires[gate.a];
        bool b = wires[gate.b];
        bool v;

        switch (gate.logic) {
            case Logic::And: v = a && b; break;
            case Logic::Or: v = a || b; break;
            case Logic::Xor: v = a != b; break;
        }

        wires[out] = v;
    }

    return get_value(wires, "z");
}

int main() {
    std::ifstream input("../24/input.txt");
    if (!input.is_open()) {
        throw std::runtime_error("Could not read file");
    }

    std::string file_content((std::istreambuf_iterator(input)), std::istreambuf_iterator<char>());
    auto delimiter_pos = file_content.find("\n\n");
    std::string wires_content = file_content.substr(0, delimiter_pos);
    std::string gates_content = file_content.substr(delimiter_pos + 2);

    std::unordered_map<std::string, bool> wires;
    std::istringstream wires_stream(wires_content);
    std::string line;

    while (std::getline(wires_stream, line)) {
        auto colon_pos = line.find(": ");
        std::string name = line.substr(0, colon_pos);
        bool value = line.substr(colon_pos + 2) == "1";
        wires[name] = value;
    }

    std::vector<Gate> gates;
    std::istringstream gates_stream(gates_content);

    while (std::getline(gates_stream, line)) {
        auto arrow_pos = line.find(" -> ");
        std::string left = line.substr(0, arrow_pos);
        std::string out = line.substr(arrow_pos + 4);

        std::istringstream left_stream(left);
        std::vector<std::string> parts;
        std::string part;

        while (left_stream >> part) {
            parts.push_back(part);
        }

        Logic logic;
        if (parts[1] == "AND") {
            logic = Logic::And;
        } else if (parts[1] == "OR") {
            logic = Logic::Or;
        } else if (parts[1] == "XOR") {
            logic = Logic::Xor;
        } else {
            throw std::runtime_error("Invalid gate: " + left);
        }

        gates.push_back({logic, parts[0], parts[2], out});
    }

    uint64_t total = run(wires, gates, {});
    std::cout << total << std::endl;

    std::unordered_map<std::string, std::vector<std::string>> edges;
    for (const auto& gate : gates) {
        edges[gate.a].push_back(gate.out);
        edges[gate.b].push_back(gate.out);
    }

    std::unordered_set<std::string> broken_nodes;

    for (const auto&[logic, a, b, out] : gates) {
        if (out.starts_with("z") && out != "z45" && logic != Logic::Xor) {
            broken_nodes.insert(out);
        }
        if (a.starts_with("z")) {
            broken_nodes.insert(a);
        }
        if (b.starts_with("z")) {
            broken_nodes.insert(b);
        }
        if (logic == Logic::Xor && !out.starts_with("z") &&
            !((a.starts_with("x") && b.starts_with("y")) ||
              (a.starts_with("y") && b.starts_with("x")))) {
            broken_nodes.insert(out);
        }
        if (logic == Logic::Xor && !out.starts_with("z") && edges[out].size() != 2) {
            broken_nodes.insert(out);
        }
        if (logic == Logic::And && !out.starts_with("z") && edges[out].size() != 1 &&
            !(a == "x00" && b == "y00") && !(a == "y00" && b == "x00")) {
            broken_nodes.insert(out);
        }
    }

    std::vector broken_nodes_list(broken_nodes.begin(), broken_nodes.end());
    std::ranges::sort(broken_nodes_list);
    for (const auto& node : broken_nodes_list) {
        std::cout << node << ",";
    }
    std::cout << std::endl;

    return 0;
}
