#include <array>
#ifndef COORDINATES_H
#define COORDINATES_H
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

struct Coordinates
{
    double lat;
    double lng;
};

const std::array<directions, 8> getAllDirections();
const std::pair<int, int> getShiftingOnDirections(const directions d);

#endif