#include "../include/Wind.h"
#include <valarray>

constexpr double pi() { return std::atan(1) * 4; }

Wind::Wind(directions initialDirection, float w, int moisture_percentage)
    : direction_(initialDirection), wind_speed_(w), moisture_percentage_(moisture_percentage)
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

int Wind::getMoistureCoeff() const
{
    return moisture_percentage_;
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
    return directionDifference(d) * numberOfDegreesPerDirection();
}

int Wind::directionDifference(directions d) const
{
    int direction_1 = static_cast<int>(direction_);
    int direction_2 =  static_cast<int>(d);
    
    if (direction_1 > direction_2)
    {
        return std::min(direction_1 - direction_2, 8-direction_1+direction_2);
    } else {
        return std::min(direction_2 - direction_1, 8-direction_2+direction_1);
    };   
}