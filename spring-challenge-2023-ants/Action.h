#ifndef ACTION_H
#define ACTION_H

#pragma once

#include <iostream>

class Action
{
public:

    enum ActionType {
        WAIT = 0,
        LINE = 1,
        BEACON = 2
    };
    Action(ActionType t, int _a,int _b, int _s):type(t),a(_a),b(_b),strength(_s){}
    ~Action(){}
    
    ActionType type;
    int a;
    int b;
    int strength;

    bool operator == (const Action& other) const {
        return type == other.type && a == other.a && b == other.b;
    }

    // operator << overloading
    friend std::ostream& operator<<(std::ostream& os, const Action& action)
    {
        switch(action.type){
            case WAIT:
                os << "WAIT;";
                break;
            case LINE:
                os << "LINE " << action.a << " " << action.b << " 1;";// << action.strength << ";";
                break;
            case BEACON:
                os << "BEACON " << action.a << " " << action.b << " 1;";// << action.strength << ";";
                break;
        }
        return os;
    }
private:

};

#endif