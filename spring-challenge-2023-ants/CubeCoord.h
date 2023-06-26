#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

class CubeCoord {
    static std::vector<std::vector<int>> directions;
    static CubeCoord CENTER;

    int x, y, z;

public:
    CubeCoord(int x, int y, int z) : x(x), y(y), z(z) {}

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    int getZ() const {
        return z;
    }

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
        int nx = x + directions[orientation][0] * distance;
        int ny = y + directions[orientation][1] * distance;
        int nz = z + directions[orientation][2] * distance;

        return CubeCoord(nx, ny, nz);
    }

    std::vector<CubeCoord> neighbours() const {
        std::vector<CubeCoord> neighborCoords;
        neighborCoords.reserve(directions.size());

        for (const auto& direction : directions) {
            CubeCoord next = neighbor(direction[3], 1);
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
