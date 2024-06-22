#pragma once
#include <cmath>
#include "Cell.h"
class Math
{
private:
    
public:
    Math();
    //virtual double calculateKoef(float windSpeed, double slopeAngleRad) const = 0;
    //virtual double calculateWindKoef(const cell* c, directions InvestigatedDirection) const = 0;
    virtual bool willSpread(const cell* c, directions InvestigatedDirection) const = 0;
    virtual bool willSpreadThroughOne(const cell* c, directions InvestigatedDirection) const = 0;
    ~Math();
};

constexpr int ignitionPercentage(){
    return 150; // in formula there is smth wrong I suppose
}

constexpr int throughPercentage(){
    return 100; // in formula there is smth wrong I suppose
}

/// @brief by now is from
/// "Aildland fire spread modelling using cellular 
/// automata: evolution in large-scale spatially heterogeneous environments 
/// under fire suppression tactics"
class Math1 final: public Math
{
private:
    double calculateKoef(float windSpeed, double slopeAngleRad) const;
    double calculateWindKoef(const cell* c, directions InvestigatedDirection) const;
public:
    bool willSpread(const cell* c, directions InvestigatedDirection) const override;
    bool willSpreadThroughOne(const cell* c, directions InvestigatedDirection) const override;
    Math1() = default;
    ~Math1() = default;
};

