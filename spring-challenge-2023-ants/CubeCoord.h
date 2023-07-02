#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


class CubeCoord {

    static std::vector<CubeCoord> directions; 
    static CubeCoord CENTER;

public:
    int q, r, s;

public:
    CubeCoord(int x, int y, int z) : q(x), r(y), s(z) {}

    bool operator==(const CubeCoord& other) const {
        return q == other.q && r == other.r && s == other.s;
    }

    bool operator!=(const CubeCoord& other) const {
        return !(*this == other);
    }

    CubeCoord neighbor(int orientation) const {
        return neighbor(orientation, 1);
    }

    CubeCoord neighbor(int orientation, int distance) const {
        int nx = q + directions[orientation].q * distance;
        int ny = r + directions[orientation].r * distance;
        int nz = s + directions[orientation].s * distance;

        return CubeCoord(nx, ny, nz);
    }

    std::vector<CubeCoord> neighbours() const {
        std::vector<CubeCoord> neighborCoords;
        neighborCoords.reserve(directions.size());

        for (int i = 0;i < (int)directions.size();i++) {
            CubeCoord next = neighbor(i, 1);
            neighborCoords.push_back(next);
        }

        return neighborCoords;
    }

    int distanceTo(const CubeCoord& dst) const {
        return (std::abs(q - dst.q) + std::abs(r - dst.r) + std::abs(s - dst.s)) / 2;
    }

    CubeCoord getOpposite() const {
        return CubeCoord(-q, -r, -s);
    }
};
