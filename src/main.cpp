#include <iostream>
#include <time.h>
#include <random>
#include <fstream>
#include "../include/Cell.h"
#include "../include/CellStorage.h"
#include "../include/Fire.h"
#include "../include/Wind.h"
#include "../include/Properties.h"
#include "../include/Connection.h"
#include "../include/Math.h"
#include "../include/Metric.h"
#include "../include/ProfilingDecorator.h"

// #include <windows.h>
// #include <Psapi.h>
#include <stdio.h>
// #include <tchar.h>

int main()
{

    printf("Size of cell from %.2ld to %.2ld bytes \n", sizeof(cell), sizeof(cell) + sizeof(Fire) + sizeof(Wind));
    Connection c = Connection();
    Math formula = Math();
    auto formulaDecorator = ProfilingDecorator(&formula);
    CellStorage s = CellStorage(&formulaDecorator);
    auto xRange = std::make_pair<int, int>(0, getXArea());
    auto yRange = std::make_pair<int, int>(0, getYArea() / 2);
    // auto w = std::make_shared<const Wind>(directions::SouthWest, float(2.0), 20); //Surgut
    auto w = std::make_shared<const Wind>(directions::NorthWest, float(6.0), 45); // Vanderhoof

    s.setWindToArea(xRange, yRange, w);

    yRange = std::make_pair<int, int>(getYArea() / 2, getYArea());
    w = std::make_shared<const Wind>(directions::SouthWest, float(2.0), 20);
    s.setWindToArea(xRange, yRange, w);

    // for (size_t i = 0; i < 3; i++)
    // {

    c.setStatesToStorage(s);

    clock_t tStart = clock();
    for (size_t i = 0; i < numberOfSimulations(); i++)
    {
        s.iterate();
        // if ((i % 10 == 0) && (i != 0))
        // {
        //     std::cout << "we have calculated " << i << " iterations" << std::endl;
        // }
        // s.printCurrentStates();
    }
    // PROCESS_MEMORY_COUNTERS memCounter;
    // K32GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));
    // std::cout << "WorkingSetSize " << memCounter.PeakWorkingSetSize << std::endl;

    printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

    // };
    s.printCurrentStates();
    // s.saveFiresToJson();

    Metric *m = new JaccardMetric();
    // auto burnt = std::vector<std::pair<int, int>>();
    // burnt.push_back(std::make_pair<int, int>(2, 3));
    // burnt.push_back(std::make_pair<int, int>(3, 4));

    // auto fired = std::vector<std::pair<int, int>>();
    // fired.push_back(std::make_pair<int, int>(5, 8));
    // fired.push_back(std::make_pair<int, int>(9, 10));
    CellStorage other_s = CellStorage(&formulaDecorator);
    other_s.uploadFromTxt();
    other_s.iterate();
    // other_s.printCurrentStates();
    clock_t tStartMetrics = clock();
    // m->calculateVariables(s, fired, burnt);

    m->calculateVariablesFrom2Storages(s, other_s);
    printf("Jaccard metric is equal to: %f\n", m->compute());
    auto simpson_metric = new SimpsonMetric(*m);
    printf("%s metric is equal to: %f\n", simpson_metric->metricName(), simpson_metric->compute());
    auto sneath_metric = new SneathMetric(*m);
    printf("%s metric is equal to: %f\n", sneath_metric->metricName(), sneath_metric->compute());
    m->printConfusionMatrix();
    printf("Time taken: %.2fs\n", (double)(clock() - tStartMetrics) / CLOCKS_PER_SEC);

    std::ofstream outFile("compare.txt");

    if (!outFile.is_open())
    {
        std::cout << "Smth goes wrong in writing in file" << std::endl;
    }

    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            if (s.getState(i, j) == cellState::Fire)
            {
                outFile << "1";
            }
            else if (other_s.getState(i, j) == cellState::Fire)
            {
                outFile << "2";
            }
            else
            {
                outFile << "0";
            };
            // if (static_cast<int>(s.getState(i, j)) == static_cast<int>(other_s.getState(i, j)))
            // {
            //     outFile << "1";
            // }
            // else
            // {
            //     outFile << "0";
            // }
        }
        outFile << '\n';
    };
    outFile << std::endl;
    outFile.close();

    // for (size_t i = 0; i < getXArea(); i++)
    // {
    //     for (size_t j = 0; j < getYArea(); j++)
    //     {
    //         if (s.getState(i,j) == cellState::Fire)
    //         {
    //             printf("%ld,%ld\n",i,j);
    //         }
            
    //     }
        
    // }
    
    return 0;
}
