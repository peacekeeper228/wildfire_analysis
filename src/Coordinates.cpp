#include <utility>
#include <array>
#include "../include/Coordinates.h"

const std::pair<int, int> getShiftingOnDirections(const directions d)
{
    std::array<std::pair<int, int>, 8> a = {
        std::make_pair(0, 1),
        std::make_pair(1, 1),
        std::make_pair(1, 0),
        std::make_pair(1, -1),
        std::make_pair(0, -1),
        std::make_pair(-1, -1),
        std::make_pair(-1, 0),
        std::make_pair(-1, 1),

    };
    return a[static_cast<int>(d)];
}

const std::array<directions, 8> getAllDirections()
{
    return std::array<directions, 8>{
        directions::North,
        directions::NorthEast,
        directions::East,
        directions::SouthEast,
        directions::South,
        directions::SouthWest,
        directions::West,
        directions::NorthWest};
}
