#include <iostream>
#include <cassert>
#include "../include/Cell.h"
#include "../include/CellStorage.h"
#include "../include/Wind.h"
#include <limits>

constexpr double tolerance(){
    return std::pow(10, -10);
}

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

void test_Wind(){
    float wS = 1.0;
    Wind w = Wind(directions::North, wS);
    assert(w.getWindDirection() == directions::North);
    
    assert(w.angleBetweenDirections(directions::North) == 0);
    assert(w.angleBetweenDirections(directions::SouthEast) == 135);
    assert(w.angleBetweenDirections(directions::West) == 90);
 
    assert(fabs(w.CalculateWindKoef(directions::East) - 1) < tolerance());
    //was calculated by hand
    //TO DO: calculate by hand
    assert(fabs(w.CalculateWindKoef((directions::North)) - 0.63147451510646979) < tolerance());
}

int main()
{
    test_Cell();  
    std::cout << "cell was tested successfully" << std::endl;
    test_CellStorage();  
    std::cout << "cellstorage was tested successfully" << std::endl;
    test_Wind();  
    std::cout << "wind was tested successfully" << std::endl;
    std::cout << "all tests passed" << std::endl;
    return 0;
}
