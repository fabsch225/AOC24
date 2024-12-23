//
// Created by fabia on 23.12.2024.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>

class Tree {
public:
    struct Node {
        std::unordered_map<char, Node*> children;
        bool is_end_of_word = false;
    };

    Tree() { root = new Node(); }

    void insert(const std::string& word) {
        Node* current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new Node();
            }
            current = current->children[c];
        }
        current->is_end_of_word = true;
    }

    std::vector<size_t> common_prefix_lengths(const std::string& design) const {
        std::vector<size_t> lengths;
        Node* current = root;
        for (size_t i = 0; i < design.size(); ++i) {
            char c = design[i];
            if (current->children.find(c) == current->children.end()) {
                break;
            }
            current = current->children[c];
            if (current->is_end_of_word) {
                lengths.push_back(i + 1);
            }
        }
        return lengths;
    }

private:
    Node* root;
};

size_t dfs(const std::string& design, const Tree& patterns, std::vector<size_t>& cache) {
    if (design.empty()) {
        return 1;
    }

    size_t index = design.size() - 1;
    if (cache[index] != SIZE_MAX) {
        return cache[index];
    }

    size_t result = 0;
    for (size_t length : patterns.common_prefix_lengths(design)) {
        result += dfs(design.substr(length), patterns, cache);
    }

    cache[index] = result;
    return result;
}

int main() {
    std::ifstream file("../19/in.txt");

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    std::istringstream pattern_stream(lines[0]);
    std::string pattern;
    std::vector<std::string> patterns;
    while (std::getline(pattern_stream, pattern, ',')) {
        pattern.erase(std::remove(pattern.begin(), pattern.end(), ' '), pattern.end());
        patterns.push_back(pattern);
    }

    Tree trie;
    for (const auto& p : patterns) {
        trie.insert(p);
    }

    size_t total1 = 0;
    size_t total2 = 0;
    std::vector<size_t> cache(100, SIZE_MAX);

    for (size_t i = 2; i < lines.size(); ++i) {
        const std::string& design = lines[i];
        cache.assign(std::max(cache.size(), design.size()), SIZE_MAX);
        size_t count = dfs(design, trie, cache);
        if (count > 0) {
            total1 += 1;
            total2 += count;
        }
    }

    std::cout << total1 << std::endl;
    std::cout << total2 << std::endl;

    return 0;
}