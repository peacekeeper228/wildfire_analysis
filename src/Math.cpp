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

double Math1::calculateWindKoef(const cell *c, directions InvestigatedDirection) const
{
    auto wind = c->getWind().get();
    float angleRadians = wind->angleBetweenDirections(InvestigatedDirection) * pi() / 180;
    return std::exp(wind->getWindSpeed() * (cos(cos(angleRadians)) - 1));
}

bool Math1::willSpread(const cell *c, directions InvestigatedDirection) const
{
    double fireKoeff = calculateWindKoef(c, InvestigatedDirection);
    return int(fireKoeff * 100) + (rand() % 100) > ignitionPercentage();
}

bool Math1::willSpreadThroughOne(const cell *c, directions InvestigatedDirection) const
{
    double fireKoeff = calculateWindKoef(c, InvestigatedDirection);
    return int(fireKoeff * 100) + (rand() % 30) > throughPercentage();
}
