#include "../include/Math.h"

Math::Math(/* args */)
{
}

Math::~Math()
{
}

inline double Math1::calculateKoeff(float windSpeed, double slopeAngleRad)
{
    double windKoeff = pow(196, 0.0133 * windSpeed);
    double slopeKoeff = 5.275 * tan(slopeAngleRad);
    return windKoeff * slopeKoeff;
}

Math1::Math1(/* args */)
{
}
