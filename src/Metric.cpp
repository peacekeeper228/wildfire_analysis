#include <algorithm>

#include "../include/Metric.h"
#include "../include/Properties.h"

Metric::Metric(/* args */)
{
}

Metric::~Metric()
{
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
            auto state = cellStorage.getState(i, j);
            auto other_state = other.getState(i, j);
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
    }
}

double SimpsonMetric::compute() const
{
    return double(a) / double(std::min(a + b, a + c));
}

double JaccardMetric::compute() const
{
    return double(a) / double(a + b + c);
}

double SneathMetric::compute() const
{
    return double(a) / double(a + 2 * b + 2 * c);
}