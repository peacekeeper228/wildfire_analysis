#include "../include/CellStorage.h"
#include "../include/Coordinates.h"
#include <stdlib.h>

CellStorage::CellStorage(/* args */)
{   
    time_after = 0;
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; i < getYArea(); i++)
        {
            Terrain[i][j] = cell();
        }
    }
}

CellStorage::~CellStorage()
{
}

void CellStorage::iterate()
{
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {   
            if (getState(i, j) == cellState::Fire){
                continue;
            }
            if (getState(i, j) == cellState::Tree){
                auto cellNeighbours = getNeighbors(i, j);
                for (auto const& k : cellNeighbours)
                {
                    if (k->getState() == cellState::Fire){
                        if (rand() % 100 > ignitionPercentage()){
                            setNewState(cellState::Fire, i, j);
                        }
                    }
                }
                
            }
        }
    }
    time_after++;
}



std::list<const cell *> CellStorage::getNeighbors(int xValue, int yValue) const
{   
    auto cellList = std::list<const cell *>();
    for (auto i : getAllDirections()){
        auto x = getShiftingOnDirections(i);
        auto a = checkAndGetCell(xValue + x.first, yValue + x.second);
        if (a != nullptr)
        {
            cellList.push_back(a);
        }
    }
    return cellList;
}
void CellStorage::setNewState(const cellState &state, int xValue, int yValue)
{
    Terrain[xValue][yValue].setState(state);
}

cellState CellStorage::getState(int xValue, int yValue) const
{
    return Terrain[xValue][yValue].getState();
}
const cell* CellStorage::checkAndGetCell(int xValue, int yValue) const
{
    if (xValue > 0 && xValue < getXArea() && yValue > 0 && yValue < getYArea()){
        return &(Terrain[xValue][yValue]);
    }
    return nullptr;
}
