#include <algorithm>

#include "../include/Metric.h"
#include "../include/Properties.h"
#include "Metric.h"

void Metric::hardResolver(cellState state, cellState other_state)
{
    if ((state == cellState::Fire and other_state == cellState::Fire) or (state == cellState::Burnt and other_state == cellState::Burnt))
    {
        ++a;
    }
    else if (state == cellState::Fire or state == cellState::Burnt)
    {
        ++b;
    }
    else if (other_state == cellState::Fire or other_state == cellState::Burnt)
    {
        ++c;
    }
    else
    {
        ++d;
    }
}

void Metric::softResolver(cellState state, cellState other_state)
{
    if ((state == cellState::Fire or state == cellState::Burnt) and (other_state == cellState::Fire or other_state == cellState::Burnt))
    {
        ++a;
    }
    else if (state == cellState::Fire or state == cellState::Burnt)
    {
        ++b;
    }
    else if (other_state == cellState::Fire or other_state == cellState::Burnt)
    {
        ++c;
    }
    else
    {
        ++d;
    }
}

Metric::Metric(/* args */)
{
}

// Metric::Metric(const Metric *other)
//     :a(other->a), b(other->b), c(other->c), d(other->d)
// {

// }

Metric::~Metric()
{
}

void Metric::printConfusionMatrix()
{
    printf("TP: %ld, FP: %ld, FN: %ld, TN: %ld\n", a, b, c, d);
}

void Metric::calculateVariables(const CellStorage &cellStorage, std::vector<std::pair<int, int>> &realFirePoints, std::vector<std::pair<int, int>> &burntPoints)
{
    int64_t allArea = getXArea() * getYArea();
    auto simulatedFirePoints = cellStorage.getRelativeFirePoints();

    auto startingPosition = simulatedFirePoints.begin();
    for (const auto &coordinates : realFirePoints)
    {
        auto lower = std::lower_bound(startingPosition, simulatedFirePoints.end(), coordinates);
        if (lower == simulatedFirePoints.end())
        {
            continue;
        }
        if ((*lower) == coordinates)
        {
            ++a;
        }
        else
        {
            ++c;
        };
        startingPosition = lower;
    }
    b = simulatedFirePoints.size() + realFirePoints.size() - a - c;
    d = allArea - a - b - c - d;
}

void Metric::calculateVariablesFrom2Storages(const CellStorage &cellStorage, const CellStorage &other)
{
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            this->softResolver(cellStorage.getState(i, j), other.getState(i, j));
            // this->hardResolver(cellStorage.getState(i, j), other.getState(i, j));
        }
    }
}

double SimpsonMetric::compute() const
{
    return double(a) / double(std::min(a + b, a + c));
}

SimpsonMetric::SimpsonMetric(const Metric &m)
    : Metric(m) {}

const char *SimpsonMetric::metricName() const
{
    return "Simpson metric";
}

double JaccardMetric::compute() const
{
    return double(a) / double(a + b + c);
}

JaccardMetric::JaccardMetric(const Metric &m)
    : Metric(m) {}

const char *JaccardMetric::metricName() const
{
    return "Jaccard metric";
}

double SneathMetric::compute() const
{
    return double(a) / double(a + 2 * b + 2 * c);
}

SneathMetric::SneathMetric(const Metric &m)
    : Metric(m) {}

const char *SneathMetric::metricName() const
{
    return "Sneath metric";
}