#pragma once
#include <cmath>
#include "Cell.h"
class Math
{
private:
    
public:
    Math();
    virtual double calculateKoef(float windSpeed, double slopeAngleRad) const = 0;
    virtual double CalculateWindKoef(const cell* c, directions InvestigatedDirection) const = 0;
    ~Math();
};



class Math1 final: public Math
{
private:
    /* data */
public:
    double calculateKoef(float windSpeed, double slopeAngleRad) const override;
    double CalculateWindKoef(const cell* c, directions InvestigatedDirection) const override;
    Math1(/* args */);
    ~Math1();
};

