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
    ~Wind();
    directions getWindDirection() const;

    /// @brief calculate the wind coeff based on mathematical model
    /// @param InvestigatedDirection direction in which koeff is investigated
    /// @return calculated coefficient
    double CalculateWindKoef(directions InvestigatedDirection) const;


    /// @brief calculate angle between direction of this wind and the direction in parameter
    /// @param d direction with which angle is calculated
    /// @return angle in degrees
    int angleBetweenDirections(directions d) const;

};
