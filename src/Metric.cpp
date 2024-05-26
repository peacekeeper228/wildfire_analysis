#include <algorithm>

#include "../include/Metrics.h"
#include "../include/Properties.h"

Metric::Metric(/* args */)
{
}

Metric::~Metric()
{
}

void Metric::calculateVariables(const CellStorage &cellStorage, std::vector<std::pair<int, int>> realFirePoints, std::vector<std::pair<int, int>> burntPoints)
{
    int a, b, c, d = 0; // naming according formulas
    int64_t allArea = getXArea() * getYArea();
    auto simulatedFirePoints = cellStorage.getRelativeFirePoints();

    auto startingPosition = simulatedFirePoints.begin();
    for (const auto &coordinates : realFirePoints)
    {
        auto lower = std::lower_bound(startingPosition, simulatedFirePoints.end(), coordinates);
        if (*lower == coordinates)
        {
            ++a;
        } else {
            ++c;
        }
        
    }
    this->a = a;
    this->b = simulatedFirePoints.size() + realFirePoints.size() - a - c;
    this->c = c;
    this->d = allArea - a - b - c - d;
}