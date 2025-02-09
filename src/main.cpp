#include <iostream>
#include <time.h>
#include <random>
#include "../include/Cell.h"
#include "../include/CellStorage.h"
#include "../include/Fire.h"
#include "../include/Wind.h"
#include "../include/Properties.h"
#include "../include/Connection.h"
#include "../include/Math.h"
#include "../include/Metric.h"

//#include <windows.h>
//#include <Psapi.h>
#include <stdio.h>
//#include <tchar.h>

int main()
{

    printf("Size of cell from %.2ld to %.2ld bytes \n", sizeof(cell), sizeof(cell) + sizeof(Fire) + sizeof(Wind));

    Connection c = Connection();
    Math1 formula = Math1();
    CellStorage s = CellStorage(&formula);
    s.latitudeMin = 10.0;
    s.longtitudeMin = 30.0;
    auto xRange = std::make_pair<int, int>(0, getXArea());
    auto yRange = std::make_pair<int, int>(0, getYArea()/2);
    auto w = std::make_shared<const Wind>(directions::North, float(1.0));
    s.setWindToArea(xRange, yRange, w);

    yRange = std::make_pair<int, int>(getYArea()/2, getYArea());
    w = std::make_shared<const Wind>(directions::SouthEast, float(2.0));
    s.setWindToArea(xRange, yRange, w);
    
    c.setStatesToStorage(s);

    clock_t tStart = clock();
    for (size_t i = 0; i < numberOfSimulations(); i++)
    {
        s.iterate();
        if ((i % 10 == 0) && (i != 0))
        {
            std::cout << "we have calculated " << i << " iterations" << std::endl;
        }
        // s.printCurrentStates();
    }
    // PROCESS_MEMORY_COUNTERS memCounter;
    // K32GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));
    // std::cout << "WorkingSetSize " << memCounter.PeakWorkingSetSize << std::endl;

    printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    s.printCurrentStates();
    // s.saveFiresToJson();

    Metric* m = new SimpsonMetric();
    auto burnt = std::vector<std::pair<int, int>>();
    burnt.push_back(std::make_pair<int, int>(2, 3));
    burnt.push_back(std::make_pair<int, int>(3, 4));

    auto fired = std::vector<std::pair<int, int>>();
    fired.push_back(std::make_pair<int, int>(5, 8));
    fired.push_back(std::make_pair<int, int>(9, 10));
    
    clock_t tStartMetrics = clock();
    m->calculateVariables(s, fired, burnt);
    std::cout <<"Simpson metric is equal to: " <<  m->compute() << std::endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStartMetrics) / CLOCKS_PER_SEC);
    // s.printCurrentStates();
    return 0;
}
