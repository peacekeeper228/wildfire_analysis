#include "../include/CellStorage.h"

CellStorage::CellStorage(/* args */)
{
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
        for (size_t j = 0; i < getYArea(); i++)
        {

            if (true)
            {
            }
        }
    }
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
