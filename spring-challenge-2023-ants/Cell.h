#ifndef CELL_H
#define CELL_H

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Cell {
    public:
        int index, cellType, resources, myAnts, oppAnts;
        vector<int> neighbors;

        Cell(int index, int cellType, int resources, vector<int> neighbors, int myAnts = 0, int oppAnts = 0) {
            this->index = index;
            this->cellType = cellType;
            this->resources = resources;
            this->neighbors = neighbors;
            this->myAnts = myAnts;
            this->oppAnts = oppAnts;
        }
};

#endif