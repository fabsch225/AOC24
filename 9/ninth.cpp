#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>

struct node {
    std::vector<int> file_ids{};
    int capacity{};
};

long checksum(std::span<node> data) {
    long sum = 0;
    size_t idx = 0;
    for (const auto&[file_ids, capacity] : data) {
        for (const auto& val : file_ids) {
            sum += idx * val;
            ++idx;
        }
    }
    return sum;
}


long part1(std::vector<node> chunks) {
    size_t left_index = 0;
    size_t right_index = chunks.size() - 1;

    while (left_index < right_index) {
        while (!(chunks[left_index].file_ids.empty() && chunks[left_index].capacity > 0)) {
            ++left_index;
        }
        while (chunks[right_index].file_ids.empty()) {
            --right_index;
        }

        if (left_index >= right_index)
            break;

        while (chunks[left_index].capacity > 0) {
            const auto val = chunks[right_index].file_ids.back();

            chunks[right_index].file_ids.pop_back();
            chunks[left_index].file_ids.push_back(val);

            --chunks[left_index].capacity;
            ++chunks[right_index].capacity;

            if (chunks[right_index].file_ids.empty()) {
                while (chunks[right_index].file_ids.empty()) {
                    --right_index;
                }
                if (left_index >= right_index)
                    break;
            }
        }
    }

    return checksum(chunks);
}

long part2(std::vector<node> chunks) {
    size_t right_index = chunks.size() - 1;

    while (right_index > 0) {
        while (chunks[right_index].file_ids.empty()) {
            --right_index;
        }

        size_t left_index = 0;
        while ((chunks[left_index].capacity - chunks[left_index].file_ids.size()) < chunks[right_index].file_ids.size() && left_index < right_index) {
            ++left_index;
        }

        if (left_index == right_index) {
            --right_index;
            continue;
        }

        chunks[left_index].file_ids.insert(chunks[left_index].file_ids.end(), chunks[right_index].file_ids.begin(), chunks[right_index].file_ids.end());
        chunks[right_index].file_ids.clear();
    }

    for (auto &node : chunks) {
        node.file_ids.resize(node.capacity);
    }

    return checksum(chunks);
}

int main() {
    std::ifstream file{"../9/disk.txt"};

    if (!file) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string str;
    std::getline(file, str);

    std::vector<node> data;
    int cur_file_id = 0;

    for (size_t idx = 0; idx < str.size(); ++idx) {
        int ch = str[idx] - '0';
        if (idx % 2 == 0) {
            data.emplace_back(node{std::vector<int>(ch, cur_file_id++), ch});
        } else {
            std::vector<int> file_ids;
            file_ids.reserve(ch);
            data.emplace_back(node{std::move(file_ids), ch});
        }
    }

    const auto p1 = part1(data);
    std::cout << "Sum = " << p1 << "\n";
    const auto p2 = part2(data);
    std::cout << "Sum = " << p2 << "\n";
}
