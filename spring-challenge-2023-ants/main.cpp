#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Board.h"

#include "utils/debug.h"
#include "utils/draw.h"

using namespace std;

void addAction(vector<Action>& actions, Action action){
    // find reference to action in actions
    if( action.strength == 0) return;
    for(auto& a : actions){
        if(a == action){
            a.strength = std::max(a.strength,action.strength);
            return;
        }
    }
    
    actions.push_back(action);
}


enum EGamePhase{
    OPENING,
    MIDGAME,
    ENDGAME,
    DEFAULT,
};

int main()
{
    if(INPUT_ON) freopen("./utils/logs/input.txt", "r", stdin);

    EGamePhase gamePhase = EGamePhase::OPENING;
    Board board;

    //////////////////


    board.init();
    draw_init(board);
    // game loop
    while (1) {
        board.read();
        drawArena(board);
        cout << "WAIT" << endl;
    }
}