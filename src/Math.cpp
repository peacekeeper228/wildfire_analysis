#include "../include/Math.h"
#include "../include/Properties.h"

Math::Math(/* args */)
{
}

Math::~Math()
{
}

double Math1::calculateKoef(float windSpeed, double slopeAngleRad) const
{
    double windKoeff = pow(196, 0.0133 * windSpeed);
    double slopeKoeff = 5.275 * tan(slopeAngleRad);
    return windKoeff * slopeKoeff;
}

double Math1::CalculateWindKoef(const cell* c, directions InvestigatedDirection) const
{
    /*
    by now is from
    "A Novel Method of Modeling Grassland Wildfire Dynamics Based on Cellular Automata:
        A Case Study in Inner Mongolia, China"
    */
    auto wind = c->getWind().get();
    float angleRadians = wind->angleBetweenDirections(InvestigatedDirection) * pi() / 180;
    return std::exp(wind->getWindSpeed() * (cos(cos(angleRadians)) - 1));
}

Math1::Math1(/* args */)
{
}

Math1::~Math1(/* args */)
{
}
