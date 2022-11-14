#include "maze.h"
#include <cstdlib>
#include <algorithm>
#include <queue>

SquareMaze::SquareMaze() : width_(0), height_(0) {}

void SquareMaze::makeMaze(int width, int height) {
    width_ = width;
    height_ = height;

    // Create set representation
    sets = DisjointSets();
    sets.addelements(width * height);

    // Create squares on a grid
    squares = std::vector<std::vector<Square>>();
    squares.resize(width);
    for (int x = 0; x < width; x++) {
        squares[x].resize(height);
    }

    for (int i = 0; i < width * height; i++) {
        int x = i % width;
        int y = i / width;

        int dir = rand() % 4;
        
        if (dir == 0 && x < width - 1 && sets.find(i) != sets.find(i + 1)) { // right wall
            squares[x][y].right = true;
            sets.setunion(i, i + 1);
        } else if (dir == 1 && y < height_ - 1 && sets.find(i) != sets.find(i + width)) { // bottom wall
            squares[x][y].bottom = true;
            sets.setunion(i, i + width);
        } else if (dir == 2 && x > 0 && sets.find(i) != sets.find(i - 1)) { // left wall
            squares[x - 1][y].right = true;
            sets.setunion(i, i - 1);
        } else if (dir == 3 && y > 0 && sets.find(i) != sets.find(i - width)) { // top wall
            squares[x][y - 1].bottom = true;
            sets.setunion(i, i - width);
        } else {
            // failsafe for rand
            for (int dir = 0; dir < 4; dir++) {
                if (dir == 0 && x < width - 1 && sets.find(i) != sets.find(i + 1)) { // right wall
                    squares[x][y].right = true;
                    sets.setunion(i, i + 1);
                } else if (dir == 1 && y < height_ - 1 && sets.find(i) != sets.find(i + width)) { // bottom wall
                    squares[x][y].bottom = true;
                    sets.setunion(i, i + width);
                } else if (dir == 2 && x > 0 && sets.find(i) != sets.find(i - 1)) { // left wall
                    squares[x - 1][y].right = true;
                    sets.setunion(i, i - 1);
                } else if (dir == 3 && y > 0 && sets.find(i) != sets.find(i - width)) { // top wall
                    squares[x][y - 1].bottom = true;
                    sets.setunion(i, i - width);
                }
            }
        }
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    if (dir == 0 && x < width_ - 1) { // right wall
            return squares[x][y].right;
    } else if (dir == 1 && y < height_ - 1) { // bottom wall
            return squares[x][y].bottom;
    } else if (dir == 2 && x > 0) { // left wall
            return squares[x - 1][y].right;
    } else if (dir == 3 && y > 0) { // top wall
            return squares[x][y - 1].bottom;
    }
    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if (dir == 0) squares[x][y].right = !exists;
    else squares[x][y].bottom = !exists;
}

std::vector<int> SquareMaze::solveMaze() {
    std::queue<int> BFS;
    BFS.push(0);

    std::vector<int> visited(width_ * height_, -1);
    visited.at(0) = 0;

    std::vector<int> sizes(width_ * width_, 0);

    while (!BFS.empty()) {
        int index = BFS.front();
        BFS.pop();

        int x = index % width_;
        int y = index / width_;

        int next = index + 1;
        if (canTravel(x, y, 0) && visited[next] < 0) {
            BFS.push(next);
            visited[next] = index;
            sizes[next] = sizes[index] + 1;
        }
        int under = index + width_;
        if (canTravel(x, y, 1) && visited[under] < 0) {
            BFS.push(under);
            visited[under] = index;
            sizes[under] = sizes[index] + 1;
        }
        int before = index - 1;
        if (canTravel(x, y, 2) && visited[before] < 0) {
            BFS.push(before);
            visited[before] = index;
            sizes[before] = sizes[index] + 1;
        }
        int above = index - width_;
        if (canTravel(x, y, 3) && visited[above] < 0) {
            BFS.push(above);
            visited[above] = index;
            sizes[above] = sizes[index] + 1;
        }
    }

    int end = width_ * (height_ - 1);
    for (int j = 0; j < width_; j++) {
        if (sizes[width_ * (height_ - 1) + j] > sizes[end]) end = width_ * (height_ - 1) + j;
    }

    std::vector<int> retval;
    for (int i = end; i != 0; i = visited[i]) {
        if (visited[i] == i - 1)
            retval.push_back(0);
        if (visited[i] == i - width_)
            retval.push_back(1);
        if (visited[i] == i + 1)
            retval.push_back(2);
        if (visited[i] == i + width_)
            retval.push_back(3);
    }
    std::reverse(retval.begin(), retval.end());
    
    return retval;
}

PNG* SquareMaze::drawMaze() const {
    PNG* retval = new PNG();
    retval->resize(width_ * 10 + 1, height_ * 10 + 1);

    for (unsigned int y = 0; y < retval->height(); y++) {
        retval->getPixel(0, y) = HSLAPixel(0, 0, 0, 1);
    }
    for (unsigned int x = 10; x < retval->width(); x++) {
        retval->getPixel(x, 0) = HSLAPixel(0, 0, 0, 1);
    }

    for (int i = 0; i < width_ * height_; i++) {
        int x = i % width_;
        int y = i / width_;

        if (!squares[x][y].right) {
            for (int w = 0; w < 11; w++) {
                retval->getPixel(((i % width_) + 1) * 10, ((i / width_) * 10) + w) = HSLAPixel(0, 0, 0, 1);
            }
        }

        if (!squares[x][y].bottom) {
            for (int w = 0; w < 11; w++) {
                retval->getPixel(((i % width_) * 10) + w, ((i / width_) + 1) * 10) = HSLAPixel(0, 0, 0, 1);
            }
        }
    }

    return retval;
}

PNG* SquareMaze::drawMazeWithSolution() {
    PNG* retval = drawMaze();
    std::vector<int> solution = solveMaze();

    int x = 5;
    int y = 5;
    for (unsigned long i = 0; i < solution.size(); i++) {
        if (solution[i] == 0) {
            for(int j = 0; j <= 10; j++) {
                retval->getPixel(x + j, y) = HSLAPixel(0, 1, 0.5, 1);
            }
            x += 10;
        } else if (solution[i] == 1) {
            for(int j = 0; j <= 10; j++) {
                retval->getPixel(x, y + j) = HSLAPixel(0, 1, 0.5, 1);
            }
            y += 10;
        } else if (solution[i] == 2) {
            for(int j = 0; j <= 10; j++) {
                retval->getPixel(x - j, y) = HSLAPixel(0, 1, 0.5, 1);
            }
            x -= 10;
        } else if (solution[i] == 3) {
            for(int j = 0; j <= 10; j++) {
                retval->getPixel(x, y - j) = HSLAPixel(0, 1, 0.5, 1);
            }
            y -= 10;
        }
    }
    // exit
    x -= 4;
    for (unsigned long i = 0; i < 9; i++) {
        retval->getPixel(x + i, retval->height() - 1) = HSLAPixel(0, 0, 1, 1);
    }

    return retval;
}
