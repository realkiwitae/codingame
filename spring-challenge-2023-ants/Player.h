#ifndef PLAYER_H
#define PLAYER_H

#pragma once

#include <string>
#include <vector>

#include "Action.h"

class Player
{
public:
    Player(){};
    ~Player(){};

    std::string message = "";
    std::vector<int> anthills = {};
    std::vector<Action> actions = {};
    int points = 0;

    void reset() {
        message = "";
        anthills.clear();
        actions.clear();
        points = 0;
    }

    void addPoints(int n) {
        points += n;
    }
private:

};

#endif