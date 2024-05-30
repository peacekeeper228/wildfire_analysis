#include <algorithm>

#include "../include/Metric.h"
#include "../include/Properties.h"

Metric::Metric(/* args */)
{
}

Metric::~Metric()
{
}

void Metric::calculateVariables(const CellStorage &cellStorage, std::vector<std::pair<int, int>>& realFirePoints, std::vector<std::pair<int, int>>& burntPoints)
{
    int64_t allArea = getXArea() * getYArea();
    auto simulatedFirePoints = cellStorage.getRelativeFirePoints();

    auto startingPosition = simulatedFirePoints.begin();
    for (const auto &coordinates : realFirePoints)
    {
        auto lower = std::lower_bound(startingPosition, simulatedFirePoints.end(), coordinates);
        if (lower == simulatedFirePoints.end()){
            continue;
        }
        if ((*lower).first == coordinates.first && (*lower).second == coordinates.second)
        {
            ++a;
        } else {
            ++c;
        }
        
    }
    b = simulatedFirePoints.size() + realFirePoints.size() - a - c;
    d = allArea - a - b - c - d;
}

double SimpsonMetric::compute() const
{
    return double(a) / double(std::min(a + b, a + c));
}