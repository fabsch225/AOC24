//
// Created by fabian on 12/15/24.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Robot {
    int posX, posY;
    int velX, velY;
};

Robot parseLine(const std::string& line) {
    Robot robot;
    std::istringstream iss(line);

    std::string pos;
    getline(iss, pos, ' ');
    sscanf(pos.c_str(), "p=%d,%d", &robot.posX, &robot.posY);

    std::string vel;
    getline(iss, vel, ' ');
    sscanf(vel.c_str(), "v=%d,%d", &robot.velX, &robot.velY);

    return robot;
}

int main() {
    const int W = 101;
    const int H = 103;

    std::ifstream file("../14/input.txt");

    std::string line;
    std::vector<Robot> robots;

    while (getline(file, line)) {
        if (!line.empty()) {
            robots.push_back(parseLine(line));
        }
    }

    int seconds = 0;
    while (true) {
        std::vector<std::vector<int>> grid(H, std::vector<int>(W, 0));
        seconds++;

        bool done = false;
        for (const auto& robot : robots) {
            int nx = (robot.posX + seconds * robot.velX) % W;
            int ny = (robot.posY + seconds * robot.velY) % H;
            if (nx < 0) nx += W;
            if (ny < 0) ny += H;

            grid[ny][nx]++;
            if (grid[ny][nx] > 1) {
                done = true;
            }
        }

        if (!done) {
            std::cout << "Seconds: " << seconds << std::endl;
            for (const auto& row : grid) {
                for (int cell : row) {
                    std::cout << (cell > 0 ? '1' : '0');
                }
                std::cout << std::endl;
            }
            return 0;
        }
    }

    return 0;
}
