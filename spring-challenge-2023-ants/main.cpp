#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Cell.h"
#include "CubeCoord.h"

using namespace std;

int main()
{
    vector<Cell> cells;
    vector<int> myBases, oppBases;
    int numberOfCells; // amount of hexagonal cells in this map
    cin >> numberOfCells; cin.ignore();
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
        Cell cell(i, type, initialResources, {neigh0, neigh1, neigh2, neigh3, neigh4, neigh5}, 0, 0);
        cells.push_back(cell);
    }
    
    cells[0].init(0, 0, 0);
    bool bReady = false;
    while(!bReady){
        bReady = true;
        for(auto& c : cells){
            if(c.bReady){
                for(auto n : c.neighbors){
                    if(!cells[n].bReady){
                        cells[n].init(c.cubeCoord.neighbor(n).x, c.cubeCoord.neighbor(n).y, c.cubeCoord.neighbor(n).z);
                    }
                }
            }else{
                bReady = false;
            }
        }
    }

    int numberOfBases;
    cin >> numberOfBases; cin.ignore();
    for (int i = 0; i < numberOfBases; i++) {
        int myBaseIndex;
        cin >> myBaseIndex; cin.ignore();
        myBases.push_back(myBaseIndex);
    }
    for (int i = 0; i < numberOfBases; i++) {
        int oppBaseIndex;
        cin >> oppBaseIndex; cin.ignore();
        oppBases.push_back(oppBaseIndex);
    }

    // game loop
    while (1) {
        std::vector<string> actions = {};
        for (int i = 0; i < numberOfCells; i++) {
            int resources; // the current amount of eggs/crystals on this cell
            int myAnts; // the amount of your ants on this cell
            int oppAnts; // the amount of opponent ants on this cell
            cin >> resources >> myAnts >> oppAnts; cin.ignore();

            cells[i].resources = resources;
            cells[i].myAnts = myAnts;
            cells[i].oppAnts = oppAnts;
        }

        //TODO: choose actions to perform and add them into actions. E.g:
        for (const auto& cell : cells) {
            if (cell.resources > 0) {
                std::string action("LINE " + std::to_string(myBases[0]) + " " + std::to_string(cell.index) + " 1");
                actions.push_back(action);
            }
        }


        // To debug: cerr << "Debug messages..." << endl;
        // WAIT | LINE <sourceIdx> <targetIdx> <strength> | BEACON <cellIdx> <strength> | MESSAGE <text>
        if (actions.size() == 0){
            cout << "WAIT" << endl;
        } else {
            for(string action : actions){
                cout << action << ";";
            }
            cout << endl;
        }
    }
}