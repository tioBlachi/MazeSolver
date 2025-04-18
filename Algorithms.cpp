#include "Algorithms.h"

Algorithms::Algorithms(const std::vector<std::string>& maze) : maze(maze) {}

std::string Algorithms::pointToStr(Point p) const {
    return std::to_string(p.first) + "," + std::to_string(p.second);
}

// General function to get neighbors of a point for all algorithms
std::vector<Algorithms::Point> Algorithms::getNeighbors(Point p) const {
    static const std::vector<Point> directions = { {-1,0},{1,0},{0,-1},{0,1} };
    std::vector<Point> neighbors;
    for (auto [dx, dy] : directions) {
        int nx = p.first + dx;
        int ny = p.second + dy;
        if (nx >= 0 && ny >= 0 && nx < maze.size() && ny < maze[0].size() && maze[nx][ny] == '.') {
            neighbors.push_back({ nx, ny });
        }
    }
    return neighbors;
}

bool Algorithms::runDijkstra(Point start, Point goal) {
    visited.clear();
    path.clear();
    cameFrom.clear();

    std::unordered_map<std::string, int> weights;

    // Min-heap: pairs of (cost, point)
    std::priority_queue<std::pair<int, Point>,std::vector<std::pair<int, Point>>,std::greater<>> myHeap;

    weights[pointToStr(start)] = 0;
    myHeap.push({ 0, start });

    while (!myHeap.empty()) {
        auto [currentCost, current] = myHeap.top();
        myHeap.pop();

        visited.push_back(current);
        if (current == goal) {
            drawFinalPath(start, goal);
            return true;
        }

        for (auto neighbor : getNeighbors(current)) {
            int newWeight = currentCost + 1;
            std::string neighborKey = pointToStr(neighbor);
            if (!weights.count(neighborKey) || newWeight < weights[neighborKey]) {
                weights[neighborKey] = newWeight;
                myHeap.push({ newWeight, neighbor });
                cameFrom[neighborKey] = current;
            }
        }
    }

    return false;
}


bool Algorithms::runBFS(Point start, Point goal) {
    visited.clear();
    path.clear();
    cameFrom.clear();

    std::queue<Point> q;
    std::unordered_set<std::string> visitedSet;

    q.push(start);
    visitedSet.insert(pointToStr(start));

    while (!q.empty()) {
        Point current = q.front();
        q.pop();

        visited.push_back(current);
        if (current == goal) {
            drawFinalPath(start, goal);
            return true;
        }

        for (auto neighbor : getNeighbors(current)) {
            std::string neighborKey = pointToStr(neighbor);
            if (!visitedSet.count(neighborKey)) {
                q.push(neighbor);
                visitedSet.insert(neighborKey);
                cameFrom[neighborKey] = current;
            }
        }
    }

    return false;
}

bool Algorithms::runDFS(Point start, Point goal) {
    visited.clear();
    path.clear();
    cameFrom.clear();

    std::stack<Point> stk;
    std::unordered_set<std::string> visitedSet;

    stk.push(start);
    visitedSet.insert(pointToStr(start));

    while (!stk.empty()) {
        Point current = stk.top();
        stk.pop();

        visited.push_back(current);
        if (current == goal) {
            drawFinalPath(start, goal);
            return true;
        }

        for (auto neighbor : getNeighbors(current)) {
            std::string neighborKey = pointToStr(neighbor);
            if (!visitedSet.count(neighborKey)) {
                stk.push(neighbor);
                visitedSet.insert(neighborKey);
                cameFrom[neighborKey] = current;
            }
        }
    }

    return false;
}

void Algorithms::drawFinalPath(Point start, Point goal) {
    Point current = goal;
    while (current != start) {
        path.push_back(current);
        current = cameFrom[pointToStr(current)];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
}

const std::vector<Algorithms::Point>& Algorithms::getVisited() const {
    return visited;
}

const std::vector<Algorithms::Point>& Algorithms::getPath() const {
    return path;
}