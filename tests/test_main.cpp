#include <iostream>
#include <cassert>
#include <limits>
#include <utility>
#include "../include/Properties.h"
#include "../include/Cell.h"
#include "../include/CellStorage.h"
#include "../include/Wind.h"
#include "../include/Fire.h"
#include "../include/Math.h"
#include "../include/Metric.h"

constexpr double tolerance(){
    return std::pow(10, -10);
}

void test_Cell(){
    cell c = cell();
    assert(c.getState() == cellState::Empty);
    c.setState(cellState::Fire);
    c.setNewState();
    assert(c.getState() == cellState::Empty);
    //test ignite
    c.setState(cellState::Tree);
    c.ignite();
    c.setNewState();
    assert(c.getState() == cellState::Fire);

    c.setState(cellState::Artificial);
    c.ignite();
    assert(c.getState() == cellState::Artificial);

    float wS = 1.0;
    auto w = std::make_shared<const Wind>(directions::North, wS);
    c.setWind(w);
    assert(c.getWind().get() == w.get());
}

void test_fire_in_cell(){
    cell c = cell();
    c.setState(cellState::Tree);
    c.ignite();
    c.setNewState();
    for (size_t i = 0; i < 10; i++)
    {
        c.iterate();
        c.setNewState();
    };
    assert(c.getState() == cellState::Burnt);
    
}

void test_CellStorage_setWindToArea(){
    Math1 formula = Math1();
    CellStorage s = CellStorage(&formula);
    std::pair<int, int> p = std::make_pair<int, int>(4, 5);
    float wS = 1.0;
    auto w = std::make_shared<const Wind>(directions::North, wS);
    
    assert(!s.checkAndGetCell(4, 4)->getWind());

    s.setWindToArea(p, p, w);
    const cell *c = s.checkAndGetCell(4, 4);
    const Wind *wInCell = c->getWind().get();
    assert(wInCell->getWindDirection() == directions::North);
    c = s.checkAndGetCell(7, 7);
    assert(!c->getWind());

    std::pair<int, int> pUnreachable = std::make_pair<int, int>(4, getXArea() + 1);
    s.setWindToArea(pUnreachable, p, w);
    c = s.checkAndGetCell(4, 7);
    assert(!c->getWind().get());
}

void test_CellStorage(){
    Math1 formula = Math1();
    CellStorage s = CellStorage(&formula);
    s.setNewState(cellState::Tree, 5, 5);
    s.setNewState(cellState::Fire, 5, 5);
    s.setNewState(cellState::Water, 4, 5);
    s.iterate();
    assert(s.getState(5, 5) == cellState::Fire);
    assert(s.getState(4, 5) == cellState::Water);
    test_CellStorage_setWindToArea();
    // iterating has not tested
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

void testFire(){
    Fire fire = Fire();
    assert(fire.getState() == FireState::Cursory);
    assert(!fire.fireEnded());
    fire.iterate();
    assert(fire.getState() == FireState::FullyDeveloped);
    assert(!fire.fireEnded());
    for (size_t i = 1; i < 3; i++)
    {
        fire.iterate(); 
        assert(fire.getState() == FireState::FullyDeveloped);
        assert(!fire.fireEnded());
    }
    fire.iterate(); 
    assert(fire.getState() == FireState::Declining);
    assert(!fire.fireEnded());
    fire.iterate(); 
    assert(fire.getState() == FireState::Ended);
    assert(fire.fireEnded());
}

void testMetrics(){
    Math1 formula = Math1();
    CellStorage s = CellStorage(&formula);

    for (size_t i = 0; i < 10; i++)
    {
        s.setNewState(cellState::Burnt, 1, i);
        s.setNewState(cellState::Tree, 2, i);
        s.setNewState(cellState::Fire, 2, i);
    }
    s.iterate();
    auto burnt = std::vector<std::pair<int, int>>();
    burnt.push_back(std::make_pair<int, int>(1, 3));
    burnt.push_back(std::make_pair<int, int>(3, 4));

    auto fired = std::vector<std::pair<int, int>>();
    fired.push_back(std::make_pair<int, int>(2, 8));
    fired.push_back(std::make_pair<int, int>(9, 10));

    Metric* m = new SimpsonMetric();
    m->calculateVariables(s, fired, burnt);
    assert(m->compute() == double(1.0));
    Metric* m1 = new JaccardMetric();
    m1->calculateVariables(s, fired, burnt);
    assert(m1->compute() == double(1) / double(12));
}

int main()
{
    test_Cell();  
    std::cout << "cell was tested successfully" << std::endl;
    test_CellStorage();  
    std::cout << "cellstorage was tested successfully" << std::endl;
    test_Wind();  
    std::cout << "wind was tested successfully" << std::endl;
    testFire();
    std::cout << "fire was tested successfully" << std::endl;
    test_fire_in_cell();
    std::cout << "fire in cell tested successfully" << std::endl;
    testMetrics();
    std::cout << "metrics tested successfully" << std::endl;
    std::cout << "all tests passed" << std::endl;
    return 0;
}
