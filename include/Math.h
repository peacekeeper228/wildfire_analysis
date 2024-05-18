#pragma once

class Math
{
private:
    
public:
    Math();
    virtual  calculateWindKoeff(float windSpeed, double slopeAngleRad) = 0;
    ~Math();
};

Math::Math(/* args */)
{
}

Math::~Math()
{
}