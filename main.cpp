#include "MazeRenderer.h"
#include "MazeGenerator.h"

int main() {
    // Step 1: Generate a new maze with at least 100,000 elements
    int rows = 401;  // Must be odd to work with the maze generation algorithm
    int cols = 251;  // Same here
    int tileSize = 5;
    int windowWidth = 1800;
    int windowHeight = 800;

    MazeGenerator generator(rows, cols);
    generator.generate();

    // Step 3: Run the renderer
    MazeRenderer renderer(generator, tileSize, windowWidth, windowHeight);
    renderer.run();

    return 0;
}
