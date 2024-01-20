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
    directions getWindDirection();
    ~Wind();
    double CalculateWindKoef(directions InvestigatedDirection);
    int angleBetweenDirections(directions d);
};
