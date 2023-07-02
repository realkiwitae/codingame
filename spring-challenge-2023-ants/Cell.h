#ifndef CELL_H
#define CELL_H

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "CubeCoord.h"
#include "Player.h"

using namespace std;

class Cell {
    public:

        enum ECellType {
            EMPTY = 0,
            EGG = 1,
            CRYSTAL = 2
        };

        enum CellLocation{
            MINE,
            OPPONENT,
            CONTESTED
        };

        ECellType type = ECellType::EMPTY;
        int richness;
        int index;
        Player* anthill;
        int ants[2] = {0, 0};
        int beacons[2] = {0,0};
        int neighbors[6] = {-1,-1,-1,-1,-1,-1};
        CubeCoord coord = CubeCoord(0,0,0);
        bool bReady = false;


        Cell(int index, int cellType, int resources, std::vector<int> neighbors, int myAnts = 0, int oppAnts = 0) {
            this->index = index;
            this->type = (ECellType)cellType;
            this->richness = resources;
            for(int i=0 ;i<6;i++)this->neighbors[i] = neighbors[i];
            this->ants[0] = myAnts;
            this->ants[1] = oppAnts;
        }

        void init(int x, int y, int z) {
            coord = CubeCoord(x, y, z);
            bReady = true;
        }
};

#endif