#pragma once
#include <cmath>
class Math
{
private:
    
public:
    Math();
    virtual double calculateKoeff(float windSpeed, double slopeAngleRad) = 0;
    ~Math();
};



class Math1 final: public Math
{
private:
    /* data */
public:
    double calculateKoeff(float windSpeed, double slopeAngleRad) override;
    Math1(/* args */);
    ~Math1();
};

