#pragma once
#include "CellStorage.h"

class Metric
{
protected:
    uint64_t a, b, c, d;

public:
    Metric(/* args */);
    ~Metric();
    void calculateVariables(const CellStorage &cellStorage, std::vector<std::pair<int, int>>& realFirePoints, std::vector<std::pair<int, int>>& burntPoints);
    virtual double compute() const = 0;
};

class SimpsonMetric final : public Metric
{
public:
    double compute() const override;
};