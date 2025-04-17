#include "MazeGenerator.h"
#include <fstream>
#include <stack>
#include <random>
#include <iostream>

MazeGenerator::MazeGenerator(int intRows, int intCols) : rows(intRows), cols(intCols) {
    maze = std::vector<std::vector<char>>(rows, std::vector<char>(cols, '#'));
}

void MazeGenerator::generate() {
    if (rows % 2 == 0) 
        rows += 1;
    if (cols % 2 == 0) 
        cols += 1;

    maze = std::vector<std::vector<char>>(rows, std::vector<char>(cols, '#'));

    carve(1, 1);

    maze[0][1] = '.';  // Entry point
    maze[rows - 1][cols - 2] = '.';  // Exit
}

void MazeGenerator::saveToFile(const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Something went wrong writing " << filename << std::endl;
        return;
    }

    for (auto row : maze) {
        for (auto cell : row) {
            outfile << cell;
        }
        outfile << '\n';
    }

    outfile.close();
}

const std::vector<std::vector<char>>& MazeGenerator::getMaze() const {
    return maze;
}

// Inspired by the recursive backtracking algorithm but using a stack
// Inspired by maze escape edugator programming quiz
// https://weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracking
// https://stackoverflow.com/questions/16121593/logic-behind-a-stack-based-maze-algorithm

void MazeGenerator::carve(int start_x, int start_y) {
    std::stack<std::pair<int, int>> stk;
    stk.push({ start_x, start_y });
    maze[start_x][start_y] = '.';

    std::vector<std::pair<int, int>> directions = {
        {-2, 0}, {2, 0}, {0, -2}, {0, 2} // 
    };

    std::random_device rd;
    std::mt19937 rng(rd());

    while (!stk.empty()) {
        int x = stk.top().first;
        int y = stk.top().second;

        std::vector<std::vector<int>> neighbors;

        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            // this makes sure we only carve within the outer borders
            if ((nx > 0 && nx < rows - 1 && ny > 0 && ny < cols - 1) && maze[nx][ny] == '#') {
                int wall_x = x + dx / 2;
                int wall_y = y + dy / 2;
                neighbors.push_back({ nx, ny, wall_x, wall_y });
            }
        }

        if (!neighbors.empty()) {
            std::shuffle(neighbors.begin(), neighbors.end(), rng);
            int nx = neighbors[0][0];
            int ny = neighbors[0][1];
            int wall_x = neighbors[0][2];
            int wall_y = neighbors[0][3];

            maze[wall_x][wall_y] = '.';
            maze[nx][ny] = '.';
            stk.push({ nx, ny });
        }
        else {
            stk.pop();
        }
    }
}

