#include "../include/CellStorage.h"
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
    if (xValue>0)
    {
        cellList.push_back(&(this->Terrain[xValue - 1][yValue]));
    }
    if (yValue>0)
    {
        cellList.push_back(&(this->Terrain[xValue][yValue - 1]));
    }
    if (yValue>0 and xValue>0)
    {
        cellList.push_back(&(this->Terrain[xValue - 1][yValue - 1]));
    }

    if (xValue<getXArea() - 1)
    {
        cellList.push_back(&(this->Terrain[xValue + 1][yValue]));
    }
    if (yValue<getYArea() - 1)
    {
        cellList.push_back(&(this->Terrain[xValue][yValue + 1]));
    }
    if (yValue<getYArea() - 1 and xValue<getXArea() - 1)
    {
        cellList.push_back(&(this->Terrain[xValue + 1][yValue + 1]));
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
