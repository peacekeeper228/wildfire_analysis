#include <iostream>
#include <cassert>
#include "../include/Cell.h"
#include "../include/CellStorage.h"

void test_Cell(){
    cell c = cell();
    assert(c.getState() == cellState::Empty);
    c.setState(cellState::Fire);
    assert(c.getState() == cellState::Fire);
    //test inFire
    c.setState(cellState::Tree);
    c.inFire();
    assert(c.getState() == cellState::Fire);

    c.setState(cellState::Artificial);
    c.inFire();
    assert(c.getState() == cellState::Artificial);
}

void test_CellStorage(){
    CellStorage s = CellStorage();
    s.setNewState(cellState::Fire, 5, 5);
    s.setNewState(cellState::Water, 4, 5);
    assert(s.getState(5, 5) == cellState::Fire);
    assert(s.getState(4, 5) == cellState::Water);
}

int main()
{
    test_Cell();  
    std::cout << "cell was tested successfully" << std::endl;
    test_CellStorage();  
    std::cout << "cellstorage was tested successfully" << std::endl;
    std::cout << "all tests passed" << std::endl;
    return 0;
}
