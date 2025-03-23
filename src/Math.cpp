#include "../include/Math.h"
#include "../include/Properties.h"
#include <cmath>
#include "Math.h"

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
    if (wind == nullptr){
        return double(1);
    }
    float angleRadians = wind->angleBetweenDirections(InvestigatedDirection);// * pi() / 180;
    double result = std::exp(wind->getWindSpeed() * (cos(angleRadians) - 1));
    result_wind_[result]++;
    return result;
}

double Math1::calculateGroundSlopeKoef(directions InvestigatedDirection, int altitudeDifference) const
{
    double coef = 0;
    if (InvestigatedDirection == directions::North || InvestigatedDirection == directions::West ||
        InvestigatedDirection == directions::East || InvestigatedDirection == directions::South)
    {
        coef = atan2(static_cast<double>(altitudeDifference), static_cast<double>(cellSizeInMeters()));
    }
    else
    {
        coef = atan2(static_cast<double>(altitudeDifference), static_cast<double>(cellSizeInMeters() * sqrt(2.0)));
    }
    double result = std::exp(0.5 * coef);
    result_slope_[result]++;
    return result;
}

bool Math1::willSpread(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    double fireKoeff = calculateWindKoef(c, InvestigatedDirection);
    double slopeKoeff = calculateGroundSlopeKoef(InvestigatedDirection, altitudeDifference);
    int result = fireKoeff * slopeKoeff * 100 - 40;
    result_overall[result]++;
    return result + (rand() % 100) > ignitionPercentage();
}

bool Math1::willSpreadThroughOne(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    double fireKoeff = calculateWindKoef(c, InvestigatedDirection);
    double slopeKoeff = calculateGroundSlopeKoef(InvestigatedDirection, altitudeDifference);
    return int(fireKoeff * slopeKoeff * 100) + (rand() % 30) > throughPercentage();
}

Math1::~Math1()
{
    printf("slope profiling\n");   
    for (auto &&i : result_slope_)
    {
        printf("%2.2f;%d\n",i.first, i.second);   
    }
    printf("wind profiling\n");   
    for (auto &&i : result_wind_)
    {
        printf("%2.2f;%d\n",i.first, i.second);   
    }
    printf("overall profiling\n");   
    for (auto &&i : result_overall)
    {
        printf("%d;%d\n",i.first, i.second);   
    }
}