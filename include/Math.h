#pragma once
#include <cmath>
#include "Cell.h"
#include <map>
#include <time.h>
#include <unordered_map>

struct SlopeMetaData
{
    bool diagonal_direction_;
    int altitude_difference_;
    bool operator==(const SlopeMetaData &rhs) const
    {
        return diagonal_direction_ == rhs.diagonal_direction_ &&
               altitude_difference_ == rhs.altitude_difference_;
    }
};

struct WindMetaData
{
    int direction_difference_;
    double wind_speed_;
    bool operator==(const WindMetaData &rhs) const
    {
        return direction_difference_ == rhs.direction_difference_ &&
               wind_speed_ == rhs.wind_speed_;
    }
};

template <>
struct std::hash<SlopeMetaData>
{
    std::size_t operator()(const SlopeMetaData &key) const
    {
        size_t h1 = std::hash<int>()(key.diagonal_direction_);
        size_t h2 = std::hash<int>()(key.altitude_difference_);
        return h1 ^ (h2 << 1);
    }
};


template <>
struct std::hash<WindMetaData>
{
    std::size_t operator()(const WindMetaData &key) const
    {
        size_t h1 = std::hash<int>()(key.direction_difference_);
        size_t h2 = std::hash<int>()(key.wind_speed_);
        return h1 ^ (h2 << 1);
    }
};

class Math
{
private:
public:
    Math();
    // virtual double calculateKoef(float windSpeed, double slopeAngleRad) const = 0;
    // virtual double calculateWindKoef(const cell* c, directions InvestigatedDirection) const = 0;
    virtual bool willSpread(const cell *c, directions InvestigatedDirection, int altitudeDifference) const = 0;
    virtual bool willSpreadThroughOne(const cell *c, directions InvestigatedDirection, int altitudeDifference) const = 0;
    ~Math();
};

constexpr int ignitionPercentage()
{
    return 50; // in formula there is smth wrong I suppose
}

constexpr int throughPercentage()
{
    return 100; // in formula there is smth wrong I suppose
}

/// @brief by now is from
/// "Aildland fire spread modelling using cellular
/// automata: evolution in large-scale spatially heterogeneous environments
/// under fire suppression tactics"
class Math1 final : public Math
{
private:
    mutable std::unordered_map<SlopeMetaData, int> slope_counter_;
    mutable std::unordered_map<SlopeMetaData, double> slope_result_;
    mutable std::map<double, int> result_wind_;
    mutable std::map<int, int> result_overall;
    mutable clock_t slope_timer;
    mutable int slope_counter;
    double calculateKoef(float windSpeed, double slopeAngleRad) const;
    double calculateWindKoef(const cell *c, directions InvestigatedDirection) const;
    double calculateGroundSlopeKoef(directions InvestigatedDirection, int altitudeDifference) const;

public:
    bool willSpread(const cell *c, directions InvestigatedDirection, int altitudeDifference) const override;
    bool willSpreadThroughOne(const cell *c, directions InvestigatedDirection, int altitudeDifference) const override;
    Math1();
    ~Math1();
};
