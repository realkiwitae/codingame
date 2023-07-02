#ifndef BOARD_H
#define BOARD_H

#pragma once

#include <vector>
#include <map>

#include "Cell.h"
#include "Player.h"
#include "CubeCoord.h"

class Board
{

private:
    int** distances_cache;

public:
    Board(){};
    ~Board(){};

    int numberOfCells;
    std::map<CubeCoord, Cell> cellMap;
    std::vector<Cell> cells;
    Player players[2] = {Player(), Player()};
    int round = 0;
    Player* me = &players[0];
    Player* opp = &players[1];

    void init();
    void update();

    int distance(Cell a, Cell b){
        //return a.coordinates.distanceTo(b.coordinates);
        return distances_cache[a.index][b.index];
    }

    int distance(int a , int b){
        return distances_cache[a][b];
    }

};

#endif