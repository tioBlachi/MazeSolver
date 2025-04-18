#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

class Algorithms {
public:
    // type alias for a point in the maze with using
    // Reference -> https://en.cppreference.com/w/cpp/language/type_alias
    using Point = std::pair<int, int>;

    Algorithms(const std::vector<std::string>& maze);

    bool runDijkstra(Point start, Point goal);
    bool runBFS(Point start, Point goal);
    bool runDFS(Point start, Point goal);

    const std::vector<Point>& getVisited() const;
    const std::vector<Point>& getPath() const;

private:
    std::vector<std::string> maze;
    std::vector<Point> visited;
    std::vector<Point> path;

    std::unordered_map<std::string, Point> cameFrom;
    std::vector<Point> getNeighbors(Point p) const;
    std::string pointToStr(Point p) const;
    void drawFinalPath(Point start, Point goal);
};

#endif
