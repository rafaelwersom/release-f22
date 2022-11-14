/**
 * @file dsets.h
 * Declaraction of the Square Maze class.
 */

#pragma once

#include <vector>
#include "../lib/cs225/PNG.h"
#include "dsets.h"

using namespace cs225;

class SquareMaze
{
    private:
        struct Square {
            bool right = false;
            bool bottom = false;
            Square() = default;
        };

        DisjointSets sets;
        std::vector<std::vector<Square>> squares;

        int width_;
        int height_;
    public:
        SquareMaze();
        void makeMaze(int width, int height);
        bool canTravel(int x, int y, int dir) const;
        void setWall(int x, int y, int dir, bool exists);
        std::vector<int> solveMaze();
        PNG* drawMaze() const;
        PNG* drawMazeWithSolution();
};
