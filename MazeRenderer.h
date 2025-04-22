#ifndef MAZE_RENDERER_H
#define MAZE_RENDERER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include "Algorithms.h"
#include  "MazeGenerator.h"

class MazeRenderer {
private:
    MazeGenerator& generator;
    std::vector<std::string> maze;
    int tileSize;
    int rows, cols;
    sf::RenderWindow window;
    sf::View view;


    // UI Elements
    sf::Font font;
    sf::Text titleText;
    sf::Text selectedAlgoText;
    sf::Text startButtonText;
    sf::Text skipButtonText;
    sf::Text timerText;
    sf::Text restButtonText;

    sf::RectangleShape resetButton;
    sf::RectangleShape algoBox;
    sf::RectangleShape startButton;
    sf::RectangleShape skipButton;

    sf::Text dfs_key;
    sf::Text bfs_key;
    sf::Text dijkstra_key;

    std::vector<std::string> algorithms = { "BFS", "DFS", "Dijkstra"};
    int selectedIndex = 0;

    // Animation data
    std::vector<Algorithms::Point> visitedPoints;
    std::vector<Algorithms::Point> pathPoints;
    int visitedIndex = 0;
    int pathIndex = 0;
    bool animating = false;
    bool skipAnimation = false;


    std::chrono::steady_clock::time_point startTime;
    std::chrono::duration<float> elapsedTime;
    void updateMaze();
    void processEvents();
    void render();

public:
        MazeRenderer(MazeGenerator& gen, int tileSize, int windowWidth, int windowHeight)
        : generator(gen),
        tileSize(tileSize),
        window(sf::VideoMode(1800, 800), "Maze Solver!"),
        view(sf::FloatRect(0, 0, 1555, windowHeight))

    {
        updateMaze();
        rows = maze.size();
        cols = maze[0].size();
        window.setView(view);

        window.setFramerateLimit(60);

        font.loadFromFile("AGENCYR.TTF");

        // UI boxes and buttons
        int sidebarX = cols * 5 + 20;

        titleText.setFont(font);
        titleText.setString("v Click to Select Algorithm");
        titleText.setCharacterSize(35);
        titleText.setFillColor(sf::Color::Black);
        titleText.setPosition(sidebarX + 200, 20);

        algoBox.setSize(sf::Vector2f(260, 40));
        algoBox.setPosition(sidebarX + 200, 60);
        algoBox.setFillColor(sf::Color(245, 245, 245));
        algoBox.setOutlineThickness(2);
        algoBox.setOutlineColor(sf::Color::Black);

        selectedAlgoText.setFont(font);
        selectedAlgoText.setCharacterSize(25);
        selectedAlgoText.setFillColor(sf::Color::Black);
        selectedAlgoText.setString(algorithms[selectedIndex]);
        selectedAlgoText.setPosition(algoBox.getPosition().x + 10, algoBox.getPosition().y + 8);

        startButton.setSize(sf::Vector2f(260, 40));
        startButton.setPosition(sidebarX + 200, 120);
        startButton.setFillColor(sf::Color(100, 200, 100));

        startButtonText.setFont(font);
        startButtonText.setString("Start");
        startButtonText.setCharacterSize(25);
        startButtonText.setFillColor(sf::Color::White);
        startButtonText.setPosition(startButton.getPosition().x + 90, startButton.getPosition().y + 8);

        skipButton.setSize(sf::Vector2f(260, 40));
        skipButton.setPosition(sidebarX + 200, 180);
        skipButton.setFillColor(sf::Color(200, 100, 100));

        skipButtonText.setFont(font);
        skipButtonText.setString("Skip Animation");
        skipButtonText.setCharacterSize(25);
        skipButtonText.setFillColor(sf::Color::White);
        skipButtonText.setPosition(skipButton.getPosition().x + 40, skipButton.getPosition().y + 8);

        resetButton.setSize(sf::Vector2f(260, 40));
        resetButton.setPosition(sidebarX + 200, 240);
        resetButton.setFillColor(sf::Color(255, 100, 100));

        restButtonText.setFont(font);
        restButtonText.setString("Reset Maze");
        restButtonText.setCharacterSize(25);
        restButtonText.setFillColor(sf::Color::White);
        restButtonText.setPosition(resetButton.getPosition().x + 40, resetButton.getPosition().y + 8);

        timerText.setFont(font);
        timerText.setCharacterSize(40);
        timerText.setFillColor(sf::Color::Black);
        timerText.setPosition(sidebarX + 200, 300);
        timerText.setString("Time: 0.0s");

        dfs_key.setFont(font);
        dfs_key.setString("DFS: PURPLE");
        dfs_key.setCharacterSize(50);
        dfs_key.setFillColor(sf::Color(255, 100, 255));
        dfs_key.setPosition(sidebarX+200, 400);

        bfs_key.setFont(font);
        bfs_key.setString("BFS: RED");
        bfs_key.setCharacterSize(50);
        bfs_key.setFillColor(sf::Color(255, 100, 100));
        bfs_key.setPosition(sidebarX+200, 450);

        dijkstra_key.setFont(font);
        dijkstra_key.setString("DIJKSTRA'S: BLUE");
        dijkstra_key.setCharacterSize(50);
        dijkstra_key.setFillColor(sf::Color(100, 100, 255));
        dijkstra_key.setPosition(sidebarX+200, 500);

        visitedIndex = 0;
        pathIndex = 0;
        animating = false;
        skipAnimation = false;



    }

    void run();
};

