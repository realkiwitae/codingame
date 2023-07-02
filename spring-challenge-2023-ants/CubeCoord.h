#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


class CubeCoord {

    static std::vector<CubeCoord> directions; 
    static CubeCoord CENTER;

public:
    int x, y, z;

public:
    CubeCoord(int x, int y, int z) : x(x), y(y), z(z) {}

    bool operator==(const CubeCoord& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const CubeCoord& other) const {
        return !(*this == other);
    }

    CubeCoord neighbor(int orientation) const {
        return neighbor(orientation, 1);
    }

    CubeCoord neighbor(int orientation, int distance) const {
        int nx = x + directions[orientation].x * distance;
        int ny = y + directions[orientation].y * distance;
        int nz = z + directions[orientation].z * distance;

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
        return (std::abs(x - dst.x) + std::abs(y - dst.y) + std::abs(z - dst.z)) / 2;
    }

    CubeCoord getOpposite() const {
        return CubeCoord(-x, -y, -z);
    }
};
