#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <vector>
#include <string>

class MazeGenerator {
public:
    MazeGenerator(int rows, int cols);
    void generate();
    void saveToFile(const std::string& filename);
    const std::vector<std::vector<char>>& getMaze() const;

private:
    int rows, cols;
    std::vector<std::vector<char>> maze;
    bool is_valid(int x, int y);
    void carve(int start_x, int start_y);
};

#endif