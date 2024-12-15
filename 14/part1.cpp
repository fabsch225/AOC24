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
    int ans[4] = {0, 0, 0, 0};

    while (getline(file, line)) {
        if (!line.empty()) {
            Robot r = parseLine(line);

            int nx = (r.posX + 100 * r.velX) % W;
            int ny = (r.posY + 100 * r.velY) % H;

            if (nx < 0) nx += W;
            if (ny < 0) ny += H;
            if (nx == W / 2 || ny == H / 2) {
                continue;
            }
            if (nx < W / 2 && ny < H / 2) {
                ans[0]++;
            } else if (nx > W / 2 && ny < H / 2) {
                ans[1]++;
            } else if (nx < W / 2 && ny > H / 2) {
                ans[2]++;
            } else {
                ans[3]++;
            }
        }
    }
    std::cout << "[" << ans[0] << ", " << ans[1] << ", " << ans[2] << ", " << ans[3] << "]" << std::endl;
    std::cout << (ans[0] * ans[1] * ans[2] * ans[3]) << std::endl;

    return 0;
}
