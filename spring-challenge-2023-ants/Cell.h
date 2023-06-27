#ifndef CELL_H
#define CELL_H

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "CubeCoord.h"

using namespace std;

class Cell {
    public:

        static inline enum CellType {
            EMPTY = 0,
            CRYSTAL = 1,
            EGG = 2,

        };

        int index, cellType, resources, myAnts, oppAnts;
        bool bReady = false;
        CubeCoord cubeCoord = {0, 0, 0};
        vector<int> neighbors;

        Cell(int index, int cellType, int resources, vector<int> neighbors, int myAnts = 0, int oppAnts = 0) {
            this->index = index;
            this->cellType = cellType;
            this->resources = resources;
            this->neighbors = neighbors;
            this->myAnts = myAnts;
            this->oppAnts = oppAnts;
        }

        void init(int x, int y, int z) {
            cubeCoord = CubeCoord(x, y, z);
            bReady = true;
        }
};

#endif