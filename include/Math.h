#pragma once
#include <cmath>
#include "Cell.h"
#include <map>
class Math
{
private:
    
public:
    Math();
    //virtual double calculateKoef(float windSpeed, double slopeAngleRad) const = 0;
    //virtual double calculateWindKoef(const cell* c, directions InvestigatedDirection) const = 0;
    virtual bool willSpread(const cell* c, directions InvestigatedDirection, int altitudeDifference) const = 0;
    virtual bool willSpreadThroughOne(const cell* c, directions InvestigatedDirection, int altitudeDifference) const = 0;
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
class Math1 final:  public Math
{
private:
    mutable std::map<double, int> result_slope_;
    mutable std::map<double, int> result_wind_;
    mutable std::map<int, int> result_overall;
    double calculateKoef(float windSpeed, double slopeAngleRad) const;
    double calculateWindKoef(const cell* c, directions InvestigatedDirection) const;
    double calculateGroundSlopeKoef(directions InvestigatedDirection, int altitudeDifference) const;
public:
    bool willSpread(const cell* c, directions InvestigatedDirection, int altitudeDifference) const override;
    bool willSpreadThroughOne(const cell* c, directions InvestigatedDirection, int altitudeDifference) const override;
    Math1() = default;
    ~Math1();
};

