#include "../include/CellStorage.h"
#include "../include/Coordinates.h"
#include <stdlib.h>
#include <iostream>

CellStorage::CellStorage(/* args */)
{   
    time_after = 0;
    //Terrain.resize(getXArea(), std::vector<cell>(getYArea()));
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            Terrain[i][j] = cell();
        }
    }
   //vtr.resize(getXArea()*100, std::vector<cell>(getYArea()));
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
                Terrain[i][j].iterate();
                continue;
            }
            if (getState(i, j) == cellState::Tree){
                auto koeff = getNeighborsKoeff(i, j);
                for (auto k : koeff)
                {   
                    if (int(k * 100) * rand() % 100 > ignitionPercentage()){
                        setNewState(cellState::Fire, i, j);
                    }
                }
                /*
                auto cellNeighbours = getNeighbors(i, j);
                for (auto const& k : cellNeighbours)
                {
                    if (k->getState() == cellState::Fire){
                        if (rand() % 100 > ignitionPercentage()){
                            setNewState(cellState::Fire, i, j);
                        }
                    }
                }
                */
            }
        }
    };
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        { 
            Terrain[i][j].setNewState();
        }
    };
    time_after++;
}

std::list<double> CellStorage::getNeighborsKoeff(int xValue, int yValue) const
{
    auto listKoef = std::list<double>();
    for (auto i : getAllDirections()){
        auto x = getShiftingOnDirections(i);
        auto a = checkAndGetCell(xValue + x.first, yValue + x.second);
        if (a != nullptr && a->getState() == cellState::Fire){
            listKoef.push_back(a->getWind()->CalculateWindKoef(i));
        }
    }
    return listKoef;
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
void CellStorage::setWindToArea(const std::pair<int, int> xRange, const std::pair<int, int> yRange, const Wind* w)
{
    if ((xRange.first < 0) | (xRange.second > getXArea()) | (yRange.first < 0) | (yRange.second > getYArea())){
        return;
        // что-то надо делать
    };

    for (int i = xRange.first; i < xRange.second; i++)
    {
        for (int j = yRange.first; j < yRange.second; j++)
        {
            Terrain[i][j].setWind(w);
        }
    }
        
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
void CellStorage::printCurrentStates(){
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        { 
            std::cout << static_cast<int>(Terrain[i][j].getState()) << " ";
        }
        std::cout << std::endl;
    };
    std::cout << std::endl;
}