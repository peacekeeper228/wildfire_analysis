#include "CellStorage.h"

class Metric
{
private:
    uint64_t a, b, c, d;
public:
    Metric(/* args */);
    ~Metric();
    void calculateVariables(const CellStorage& cellStorage, std::vector<std::pair<int, int>> realFirePoints, std::vector<std::pair<int, int>> burntPoints);
    virtual double compute() = 0;
};

