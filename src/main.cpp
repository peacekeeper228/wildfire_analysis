#include <iostream>
#include "../include/Cell.h"
#include "../include/CellStorage.h"
int main()
{
    /* code */
    cell c = cell();
    std::cout << static_cast<int>(c.getState()) << std::endl;

    CellStorage s = CellStorage();
    s.setNewState(cellState::Fire, 6, 5);
    s.setNewState(cellState::Fire, 4, 5);
    auto neighbours = s.getNeighbors(5, 5);
    for (auto i : neighbours)
    {  
        if (i->getState() == cellState::Fire){
            s.setNewState(cellState::Fire, 5, 5);
        }
        std::cout << static_cast<int>(i->getState()) << std::endl;
    }

    return 0;
}
