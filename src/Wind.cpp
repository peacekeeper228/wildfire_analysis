#include "../include/Wind.h"
#include <valarray>

constexpr double pi() { return std::atan(1)*4; }

Wind::Wind(directions initialDirection, float w)
{
    this->direction = initialDirection;
    this->windSpeed = w;
}

directions Wind::getWindDirection()
{
    return this->direction;
}
Wind::~Wind()
{
}

double Wind::CalculateWindKoef(directions InvestigatedDirection)
{
    /*
    by now is from
    "A Novel Method of Modeling Grassland Wildfire Dynamics Based on Cellular Automata:
        A Case Study in Inner Mongolia, China"
    
    */
   float angleRadians = this->angleBetweenDirections(InvestigatedDirection) * pi() / 180;
   return std::exp(this->windSpeed * (cos(cos(angleRadians)) - 1));
}
int Wind::angleBetweenDirections(directions d)
{
    int differenceInDirections = abs(static_cast<int>(d) - static_cast<int>(this->direction));
    if (differenceInDirections > 4) {
        return (8 - differenceInDirections) * numberOfDegreesPerDirection();
    };
    return differenceInDirections * numberOfDegreesPerDirection();
}