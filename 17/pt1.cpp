//
// Created by fabian on 12/19/24.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <stdexcept>

using namespace std;

vector<uint64_t> run(uint64_t a, uint64_t b, uint64_t c, const vector<uint64_t>& program) {
    size_t ip = 0;
    vector<uint64_t> output;

    while (ip < program.size()) {
        uint64_t opcode = program[ip];
        uint64_t literal = program[ip + 1];

        uint64_t combo = (literal <= 3) ? literal :
                         (literal == 4) ? a :
                         (literal == 5) ? b :
                         (literal == 6) ? c : literal;

        switch (opcode) {
            case 0: a >>= combo; break;
            case 1: b ^= literal; break;
            case 2: b = combo % 8; break;
            case 3:
                if (a != 0) {
                    ip = static_cast<size_t>(literal);
                    continue;
                }
                break;
            case 4: b ^= c; break;
            case 5: output.push_back(combo % 8); break;
            case 6: b = a >> combo; break;
            case 7: c = a >> combo; break;
            default: throw runtime_error("Invalid opcode");
        }

        ip += 2;
    }

    return output;
}

int main() {
    uint64_t a = 37283687;
    uint64_t b = 0;
    uint64_t c = 0;

    vector<uint64_t> program = {2,4,1,3,7,5,4,1,1,3,0,3,5,5,3,0};

    vector<uint64_t> part1 = run(a, b, c, program);
    for (size_t i = 0; i < part1.size(); ++i) {
        cout << part1[i];
        if (i < part1.size() - 1) {
            cout << ",";
        }
    }
    cout << endl;

    vector<uint64_t> factors(program.size(), 0);

    while (true) {
        uint64_t init_a = 0;
        for (size_t i = 0; i < factors.size(); ++i) {
            init_a += static_cast<uint64_t>(pow(8, i)) * factors[i];
        }

        vector<uint64_t> output = run(init_a, b, c, program);

        if (output == program) {
            cout << init_a << endl;
            break;
        }

        for (size_t i = program.size(); i-- > 0;) {
            if (output.size() <= i || output[i] != program[i]) {
                factors[i] += 1;
                for (size_t j = 0; j < i; ++j) {
                    factors[j] = 0;
                }
                break;
            }
        }
    }

    return 0;
}
