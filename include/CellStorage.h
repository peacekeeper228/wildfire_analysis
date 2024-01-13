#ifndef CELLSTORAGE_H
#define CELLSTORAGE_H

#include "Cell.h"
#include <cstddef>
#include <list>

constexpr int getXArea(){
    return 10;
}

constexpr int getYArea(){
    return 10;
}
class CellStorage
{
private:
    /* data */
    cell Terrain[getXArea()][getYArea()]; 
public:
    void setNewState(const cellState& state, int xValue, int yValue);
    cellState getState(int xValue, int yValue) const;
    CellStorage();
    ~CellStorage();
    void iterate();
    std::list<const cell*> getNeighbors(int xValue, int yValue) const;
};

#endif