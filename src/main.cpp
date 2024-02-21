#include <iostream>
#include <time.h>
#include "../include/Cell.h"
#include "../include/CellStorage.h"
#include "../include/Fire.h"
#include "../include/Wind.h"
int main()
{
    printf("Size of cell from %.2ld to %.2ld bytes \n", sizeof(cell), sizeof(cell) + sizeof(Fire) + sizeof(Wind));

    CellStorage s = CellStorage();
    auto xRange = std::make_pair<int, int>(0, getXArea());
    auto yRange = std::make_pair<int, int>(0, getYArea());
    auto w = Wind(directions::North, float(1.0));
    s.setWindToArea(xRange, yRange, &w);
    for (int i = 0; i < getXArea(); i++)
    {
        for (int j = 0; j < getYArea(); j++)
        {  
            s.setNewState(cellState::Tree, i, j);
            if (i == 6){
                // s.setNewState(cellState::Water, i, j); 
            }
        }
        
    }
    s.setNewState(cellState::Fire, 50, 50);
    
    clock_t tStart = clock();
    for (size_t i = 0; i < 100; i++)
    {
        s.iterate();
        if (i % 100 == 0){
            std::cout << "we have calculated " << i  << " iterations" << std::endl;
        }
        // s.printCurrentStates();
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    s.printCurrentStates();
    /*
    1000х1000 на 100 итераций - ~16 секунд c ветром, размер: 24 Б на клетку
    10000x10000 на 100 итераций - 252~ секунды без ветра, размер: 24 Б на клетку 
    */
    return 0;
}
