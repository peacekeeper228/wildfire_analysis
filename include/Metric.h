#pragma once
#include "CellStorage.h"

class Metric
{
protected:
    uint64_t a, b, c, d;
    void hardResolver(cellState state, cellState other_state);
    void softResolver(cellState state, cellState other_state);

public:
    Metric(/* args */);
    // Metric(const Metric* other);
    Metric(const Metric &m) = default;
    ~Metric();
    void printConfusionMatrix();
    void calculateVariables(const CellStorage &cellStorage, std::vector<std::pair<int, int>> &realFirePoints, std::vector<std::pair<int, int>> &burntPoints);
    void calculateVariablesFrom2Storages(const CellStorage &cellStorage, const CellStorage &other);
    virtual double compute() const = 0;
    virtual const char* metricName() const = 0;
};

class SimpsonMetric final : public Metric
{
public:
    double compute() const override;
    SimpsonMetric(const Metric &m);
    SimpsonMetric() = default;
    const char* metricName() const;
};

class JaccardMetric final : public Metric
{
public:
    double compute() const override;
    JaccardMetric(const Metric &m);
    JaccardMetric() = default;
    const char* metricName() const;
};

class SneathMetric final : public Metric
{
public:
    double compute() const override;
    SneathMetric(const Metric &m);
    SneathMetric() = default;
    const char* metricName() const;
};
