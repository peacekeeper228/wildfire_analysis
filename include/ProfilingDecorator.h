#pragma once
#include <unordered_map>
#include <map>
#include "Math.h"

struct metaData
{
    directions investigated_direction_;
    int altitude_difference_;
    directions wind_direction_;
    float wind_speed_;

    bool operator==(const metaData &rhs) const
    {
        return investigated_direction_ == rhs.investigated_direction_ &&
               altitude_difference_ == rhs.altitude_difference_ &&
               wind_direction_ == rhs.wind_direction_ &&
               wind_speed_ == rhs.wind_speed_;
    }
};

template <>
struct std::hash<metaData>
{
    std::size_t operator()(const metaData &key) const
    {
        size_t h1 = std::hash<int>()(static_cast<int>(key.investigated_direction_));
        size_t h2 = std::hash<int>()(key.altitude_difference_);
        size_t h3 = std::hash<int>()(static_cast<int>(key.wind_direction_));
        size_t h4 = std::hash<float>{}(key.wind_speed_);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};

class ProfilingDecorator final : public Math
{
private:
    Math *primary_class_;
    mutable int positive_counter_;
    mutable int counter_;
    mutable std::unordered_map<metaData, int> results_;
    mutable std::unordered_map<directions, int> results_directions_;

public:
    ProfilingDecorator(Math *formula);
    bool willSpread(const cell *c, directions InvestigatedDirection, int altitudeDifference) const override;
    bool willSpreadThroughOne(const cell *c, directions InvestigatedDirection, int altitudeDifference) const override;
    ~ProfilingDecorator();
};