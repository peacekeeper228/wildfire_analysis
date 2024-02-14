#include "../include/Wind.h"
#include <valarray>

constexpr double pi() { return std::atan(1)*4; }

Wind::Wind(directions initialDirection, float w)
    :direction(initialDirection),
    windSpeed(w),
    numberOfAssignedCells(0)
{
}

directions Wind::getWindDirection() const
{
    return this->direction;
}
Wind::~Wind()
{
}

void Wind::windIsDisassignedFromCell()
{
    this->numberOfAssignedCells--;
    if (this->numberOfAssignedCells == 0){
        this->~Wind();
    }
}

void Wind::windIsAssignedToCell()
{
    this->numberOfAssignedCells++;
}

double Wind::CalculateWindKoef(directions InvestigatedDirection) const
{
    /*
    by now is from
    "A Novel Method of Modeling Grassland Wildfire Dynamics Based on Cellular Automata:
        A Case Study in Inner Mongolia, China"
    
    */
   float angleRadians = this->angleBetweenDirections(InvestigatedDirection) * pi() / 180;
   return std::exp(this->windSpeed * (cos(cos(angleRadians)) - 1));
}
int Wind::angleBetweenDirections(directions d) const
{
    int differenceInDirections = abs(static_cast<int>(d) - static_cast<int>(this->direction));
    if (differenceInDirections > 4) {
        return (8 - differenceInDirections) * numberOfDegreesPerDirection();
    };
    return differenceInDirections * numberOfDegreesPerDirection();
}