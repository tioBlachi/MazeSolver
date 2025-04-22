#include "MazeRenderer.h"
#include "Algorithms.h"
#include "MazeGenerator.h"
#include <iostream>
#include <sstream>

// heavily referencing sfml documentation to make many of these work
// https://www.sfml-dev.org/documentation/2.6.2/

void MazeRenderer::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void MazeRenderer::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }, window.getDefaultView());

            if (algoBox.getGlobalBounds().contains(mousePos)) {
                selectedIndex = (selectedIndex + 1) % algorithms.size();
                selectedAlgoText.setString(algorithms[selectedIndex]);
            }

            if (startButton.getGlobalBounds().contains(mousePos)) {
                Algorithms solver(maze);
                Algorithms::Point start = { 0, 1 };
                Algorithms::Point goal = { static_cast<int>(maze.size()) - 1, static_cast<int>(maze[0].size()) - 2 };

                // https://en.cppreference.com/w/cpp/chrono/steady_clock/now
                // how to use steady_clock
                startTime = std::chrono::steady_clock::now();

                if (algorithms[selectedIndex] == "Dijkstra")
                    solver.runDijkstra(start, goal);
                else if (algorithms[selectedIndex] == "BFS")
                    solver.runBFS(start, goal);
                else if (algorithms[selectedIndex] == "DFS")
                    solver.runDFS(start, goal);


                elapsedTime = std::chrono::steady_clock::now() - startTime;

                visitedPoints = solver.getVisited();
                pathPoints = solver.getPath();
                visitedIndex = 0;
                pathIndex = 0;
                animating = true;
                skipAnimation = false;
            }

            if (skipButton.getGlobalBounds().contains(mousePos)) {
                skipAnimation = true;
            }
            if (resetButton.getGlobalBounds().contains(mousePos)) {
                updateMaze();

            }
        }
    }

    float scrollSpeed = 40.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) view.move(0, -scrollSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) view.move(0, scrollSpeed);

    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            float scroll = event.mouseWheelScroll.delta;
            view.move(0, -scroll * scrollSpeed);
        }
    }

    // Clamp view to maze bounds, otherwise chaos
    sf::Vector2f center = view.getCenter();
    float halfWidth = view.getSize().x / 2.0f;
    float halfHeight = view.getSize().y / 2.0f;

    float clampedX = std::max(halfWidth, std::min(center.x, static_cast<float>(cols * tileSize) - halfWidth));
    float clampedY = std::max(halfHeight, std::min(center.y, static_cast<float>(rows * tileSize) - halfHeight));

    view.setCenter(clampedX, clampedY);

    window.setView(view);
}


void MazeRenderer::render() {
    window.clear();

    // Drawing the maze with white and black tiles
    sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            tile.setPosition(x * tileSize, y * tileSize);

            if (maze[y][x] == '#') {
                tile.setFillColor(sf::Color::Black);
            }
            else {
                tile.setFillColor(sf::Color::White);
            }

            window.draw(tile);
        }
    }

    // animation
    if (animating) {
        if (skipAnimation) {
            for (const auto& point : visitedPoints) {
                sf::RectangleShape v(sf::Vector2f(tileSize, tileSize));
                v.setPosition(point.second * tileSize, point.first * tileSize);

                if (algorithms[selectedIndex] == "Dijkstra")
                    v.setFillColor(sf::Color(100, 100, 255));
                else if (algorithms[selectedIndex] == "BFS")
                    v.setFillColor(sf::Color(255, 100, 100));
                else if (algorithms[selectedIndex] == "DFS")
                    v.setFillColor(sf::Color(255, 100, 255));
                window.draw(v);
            }
            for (const auto& point : pathPoints) {
                sf::RectangleShape p(sf::Vector2f(tileSize, tileSize));
                p.setPosition(point.second * tileSize, point.first * tileSize);
                p.setFillColor(sf::Color::Green);
                window.draw(p);
            }
            animating = false;
        }
        else {
            if (visitedIndex < visitedPoints.size()) {
                for (int i = 0; i <= visitedIndex; ++i) {
                    sf::RectangleShape v(sf::Vector2f(tileSize, tileSize));
                    v.setPosition(visitedPoints[i].second * tileSize, visitedPoints[i].first * tileSize);
                    if (algorithms[selectedIndex] == "Dijkstra")
                        v.setFillColor(sf::Color(100, 100, 255));
                    else if (algorithms[selectedIndex] == "BFS")
                        v.setFillColor(sf::Color(255, 100, 100));
                    else if (algorithms[selectedIndex] == "DFS")
                        v.setFillColor(sf::Color(255, 100, 255));
                    window.draw(v);
                }
                                                          visitedIndex += 20;// turn this up or down to speed up or slow down the animation, 20 seems ok
            }
            else {
                animating = false;
            }
        }
    }

    if (!animating && !pathPoints.empty()) {
        for (const auto& point : pathPoints) {
            sf::RectangleShape p(sf::Vector2f(tileSize, tileSize));
            p.setPosition(point.second * tileSize, point.first * tileSize);
            p.setFillColor(sf::Color::Green);
            window.draw(p);
        }
    }

    // === UI aREA ===
    window.setView(window.getDefaultView());

    // Sidebar for UI
    sf::RectangleShape sidebar(sf::Vector2f(400, window.getSize().y));
    sidebar.setPosition(1453, 0);
    sidebar.setFillColor(sf::Color(220, 220, 220));
    window.draw(sidebar);

    window.draw(titleText);
    window.draw(algoBox);
    window.draw(selectedAlgoText);
    window.draw(startButton);
    window.draw(startButtonText);
    window.draw(skipButton);
    window.draw(skipButtonText);
    window.draw(resetButton);
    window.draw(restButtonText);
    window.draw(dfs_key);
    window.draw(bfs_key);
    window.draw(dijkstra_key);
    std::ostringstream oss;
    oss.precision(5);
    oss << std::fixed << "Time: " << elapsedTime.count() << "s";
    timerText.setString(oss.str());
    window.draw(timerText);

    window.display();
}
void MazeRenderer::updateMaze() {
    generator.generate();
    const auto& mazeLayout = generator.getMaze();
    maze.clear();
    for (const auto& row : mazeLayout) {
        maze.emplace_back(row.begin(), row.end());
    }
    rows = maze.size();
    cols = maze[0].size();
    visitedPoints.clear();
    pathPoints.clear();
    visitedIndex = 0;
    pathIndex = 0;
    animating = false;
    skipAnimation = false;
    elapsedTime = std::chrono::duration<float>::zero();
}
