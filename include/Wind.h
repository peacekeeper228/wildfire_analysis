#pragma once
#include <cstdint>
#include "Coordinates.h"
class Wind
{
private:
    directions direction;
    float windSpeed;
    uint64_t numberOfAssignedCells;
public:
    Wind(directions initialDirection, float w);
    directions getWindDirection() const;
    ~Wind();
    void windIsAssignedToCell();
    void windIsDisassignedFromCell();
    double CalculateWindKoef(directions InvestigatedDirection) const;
    int angleBetweenDirections(directions d) const;

};
