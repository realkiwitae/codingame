#pragma once

#include <iostream>
#include <string>

#define DEBUG_ON 0
#define INPUT_ON 1

//logger with several arguments of different types
template<typename T, typename... Args>
inline void logger(T t, Args... args){
    if(!DEBUG_ON)return;
    std::cerr << t << " ";
    if constexpr (sizeof...(args) > 0) {
        logger(args...);
    }
    else{
        std::cerr << std::endl;
    }
}

// base case to terminate recursion
inline void logger() {}