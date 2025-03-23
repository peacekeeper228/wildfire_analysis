#include "../include/Wind.h"
#include <valarray>

constexpr double pi() { return std::atan(1) * 4; }

Wind::Wind(directions initialDirection, float w)
    : direction_(initialDirection), wind_speed_(w)
{
}

directions Wind::getWindDirection() const
{
    return this->direction_;
}

float Wind::getWindSpeed() const
{
    return this->wind_speed_;
}

Wind::~Wind()
{
}

double Wind::CalculateWindKoef(directions InvestigatedDirection) const
{
    /*
    by now is from
    "A Novel Method of Modeling Grassland Wildfire Dynamics Based on Cellular Automata:
        A Case Study in Inner Mongolia, China"

    */
    float angleRadians = this->angleBetweenDirections(InvestigatedDirection) * pi() / 180;
    return std::exp(this->wind_speed_ * (cos(cos(angleRadians)) - 1));
}
int Wind::angleBetweenDirections(directions d) const
{
    int differenceInDirections = abs(static_cast<int>(d) - static_cast<int>(this->direction_));
    // if (differenceInDirections > 4)
    // {
    //     return (8 - differenceInDirections) * numberOfDegreesPerDirection();
    // };
    return differenceInDirections * numberOfDegreesPerDirection();
}

