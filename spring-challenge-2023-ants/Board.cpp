#pragma once

#include "Board.h"

#include <iostream>

#include "utils/debug.h"

void Board::init(){
    cin >> numberOfCells; cin.ignore();
    logger(numberOfCells);

    for (int i = 0; i < numberOfCells; i++) {
        int type; // 0 for empty, 1 for eggs, 2 for crystal
        int initialResources; // the initial amount of eggs/crystals on this cell
        int neigh0; // the index of the neighbouring cell for each direction
        int neigh1;
        int neigh2;
        int neigh3;
        int neigh4;
        int neigh5;
        cin >> type >> initialResources >> neigh0 >> neigh1 >> neigh2 >> neigh3 >> neigh4 >> neigh5; cin.ignore();
        
        logger(type, initialResources, neigh0, neigh1, neigh2, neigh3, neigh4, neigh5);
        
        Cell cell(i, type, initialResources, {neigh0, neigh1, neigh2, neigh3, neigh4, neigh5}, 0, 0);
        cells.push_back(cell);
        allCrystals += initialResources*(type == Cell::ECellType::CRYSTAL);
    }

    // allocate memory to distances_cache
    distances_cache = new int*[numberOfCells];
    for(int i=0 ; i < numberOfCells; i++){
        distances_cache[i] = new int[numberOfCells];
        for(int j=0 ; j < numberOfCells; j++){
            distances_cache[i][j] = 99999;
        }
    }
    // use floodfill exploration from each cell to find relative distances_cache between cells store in distances_cache

    for(auto c : cells){
        //if(c.type == Cell::EMPTY) continue;
        std::vector<int> frontier;
        std::vector<int> next;
        frontier.push_back(c.index);
        distances_cache[c.index][c.index] = 0;
        int dist = 1;
        while(frontier.size() > 0){
            for(int f : frontier){
                for(int n : cells[f].neighbors){
                    if(n < 0) continue;
                    if(distances_cache[c.index][n] > dist){
                        distances_cache[c.index][n] = dist;
                        next.push_back(n);
                    }
                }
            }
            frontier = next;
            next.clear();
            dist++;
        }
    }

    cells[0].init(0, 0, 0);
    bool bReady = false;
    while(!bReady){
        bReady = true;
        for(auto& c : cells){
            if(c.bReady){
                for(int i=0 ; i < 6; i++){
                    if(c.neighbors[i] < 0) continue;
                    if(cells[c.neighbors[i]].bReady)continue;
                    cells[c.neighbors[i]].init(c.coord.neighbor(i).q, c.coord.neighbor(i).r, c.coord.neighbor(i).s);
                }
            }else{
                bReady = false;
            }
        }
    }

    int numberOfBases;
    cin >> numberOfBases; cin.ignore();
    logger(numberOfBases);
    for (int i = 0; i < numberOfBases; i++) {
        int myBaseIndex;
        cin >> myBaseIndex; cin.ignore();
        logger(myBaseIndex);
        me->anthills.push_back(myBaseIndex);
    }
    for (int i = 0; i < numberOfBases; i++) {
        int oppBaseIndex;
        cin >> oppBaseIndex; cin.ignore();
        logger(oppBaseIndex);
        opp->anthills.push_back(oppBaseIndex);
    }
}

void Board::update(){
    for (int i = 0; i < numberOfCells; i++) {
        int resources; // the current amount of eggs/crystals on this cell
        int myAnts; // the amount of your ants on this cell
        int oppAnts; // the amount of opponent ants on this cell
        cin >> resources >> myAnts >> oppAnts; cin.ignore();
        if(DEBUG_ON) cerr << resources << " " << myAnts << " " << oppAnts << endl;
        cells[i].richness = resources;
        cells[i].ants[0] = myAnts;
        cells[i].ants[1] = oppAnts;

    }
}