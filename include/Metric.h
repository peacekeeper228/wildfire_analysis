#pragma once
#include "CellStorage.h"

class Metric
{
protected:
    uint64_t a, b, c, d;

public:
    Metric(/* args */);
    Metric(const Metric& m) = default;
    ~Metric();
    void calculateVariables(const CellStorage& cellStorage, std::vector<std::pair<int, int>>& realFirePoints, std::vector<std::pair<int, int>>& burntPoints);
    void calculateVariablesFrom2Storages(const CellStorage& cellStorage, const CellStorage& other);
    virtual double compute() const = 0;
};

class SimpsonMetric final : public Metric
{
public:
    double compute() const override;
};

class JaccardMetric final : public Metric
{
public:
    double compute() const override;
};

class SneathMetric final : public Metric
{
public:
    double compute() const override;
};
