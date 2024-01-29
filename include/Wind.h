#pragma once
#include "Coordinates.h"
class Wind
{
private:
    directions direction;
    float windSpeed;
    /* data */
public:
    Wind(directions initialDirection, float w);
    directions getWindDirection() const;
    ~Wind();
    double CalculateWindKoef(directions InvestigatedDirection) const;
    int angleBetweenDirections(directions d) const;
};
