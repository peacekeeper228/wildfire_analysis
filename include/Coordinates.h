#include <array>
#pragma once
enum class directions
{
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest
};

inline int numberOfDegreesPerDirection(){
    return 45;
}

const std::array<directions, 8> getAllDirections();
const std::pair<int, int> getShiftingOnDirections(const directions d);
