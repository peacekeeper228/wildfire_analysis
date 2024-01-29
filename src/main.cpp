#include <iostream>
#include <time.h>
#include "../include/Cell.h"
#include "../include/CellStorage.h"
int main()
{
    printf("Size of cell: %.2ld bytes\n", sizeof(cell));

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
        }
        
    }
    s.setNewState(cellState::Fire, 1, 1);
    
    clock_t tStart = clock();
    for (size_t i = 0; i < 10; i++)
    {
        s.iterate();
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    
    /*
    1000х1000 на 100 итераций - ~16 секунд c ветром, размер: 24 Б на клетку
    */
    return 0;
}
