#include "../include/CellStorage.h"
#include "../include/Coordinates.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

CellStorage::CellStorage(/* args */)
{
    time_after = 0;
    Terrain.resize(getXArea(), std::vector<cell>(getYArea()));
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            Terrain[i][j] = cell();
        }
    }
    // vtr.resize(getXArea()*100, std::vector<cell>(getYArea()));
}

CellStorage::~CellStorage()
{
}

void CellStorage::iterate()
{
    for (int i = 0; i < getXArea(); i++)
    {
        for (int j = 0; j < getYArea(); j++)
        {
            if (getState(i, j) == cellState::Fire)
            {
                // we can do that because invariant is checked in cell
                if (!checkAndGetCell(i, j)->getFireInCell()->canSpread())
                {
                    //TODO: this logic is weird
                    Terrain[i][j].iterate();
                    continue;
                }
                auto listKoef = std::list<double>();
                for (auto analyzedDirection : getAllDirections())
                {
                    auto x = getShiftingOnDirections(analyzedDirection);
                    auto a = checkAndGetCell(i + x.first, j + x.second);
                    if ((a != nullptr) && (a->getState() == cellState::Tree))
                    {
                        double fireKoeff = 0;
                        if (a->getWind() != nullptr)
                        {
                            fireKoeff = a->getWind()->CalculateWindKoef(analyzedDirection);
                        }
                        // TODO calculate k
                        if (int(fireKoeff * 100) + (rand() % 100) > ignitionPercentage())
                        {
                            setNewState(cellState::Fire, i + x.first, j + x.second);
                        }
                    }
                }

                Terrain[i][j].iterate();

                // if (getState(i, j) == cellState::Fire){
                //     Terrain[i][j].iterate();
                //     continue;
                // }
                // if (getState(i, j) == cellState::Tree){
                //     auto koeff = getNeighborsKoeff(i, j);
                //     for (auto k : koeff)
                //     {
                //         if ((int(k * 100) + rand()) % 100 > ignitionPercentage()){
                //             setNewState(cellState::Fire, i, j);
                //         }
                //     }
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

// std::list<double> CellStorage::getNeighborsKoeff(int xValue, int yValue) const
// {
//     auto listKoef = std::list<double>();
//     for (auto i : getAllDirections())
//     {
//         auto x = getShiftingOnDirections(i);
//         auto a = checkAndGetCell(xValue + x.first, yValue + x.second);
//         if (a != nullptr && a->getState() == cellState::Fire)
//         {
//             if (a->getWind() != nullptr)
//             {
//                 listKoef.push_back(a->getWind()->CalculateWindKoef(i));
//             }
//             else
//             {
//                 listKoef.push_back(0);
//             }
//         }
//     }
//     return listKoef;
// }

// std::list<const cell *> CellStorage::getNeighbors(int xValue, int yValue) const
// {
//     auto cellList = std::list<const cell *>();
//     for (auto i : getAllDirections())
//     {
//         auto x = getShiftingOnDirections(i);
//         auto a = checkAndGetCell(xValue + x.first, yValue + x.second);
//         if (a != nullptr)
//         {
//             cellList.push_back(a);
//         }
//     }
//     return cellList;
// }
bool CellStorage::setWindToArea(const std::pair<int, int> xRange, const std::pair<int, int> yRange, const Wind *w)
{
    if ((xRange.first < 0) | (xRange.second > getXArea()) | (yRange.first < 0) | (yRange.second > getYArea()))
    {
        return false;
        // TODO: что-то надо делать
    };

    for (int i = xRange.first; i < xRange.second; i++)
    {
        for (int j = yRange.first; j < yRange.second; j++)
        {
            setWindToCell(&(Terrain[i][j]), const_cast<Wind *>(w));
        }
    }
    return true;
}
void CellStorage::setNewState(const cellState &state, int xValue, int yValue)
{
    Terrain[xValue][yValue].setState(state);
}

cellState CellStorage::getState(int xValue, int yValue) const
{
    return Terrain[xValue][yValue].getState();
}
const cell *CellStorage::checkAndGetCell(int xValue, int yValue) const
{
    if (xValue >= 0 && xValue < getXArea() && yValue >= 0 && yValue < getYArea())
    {
        return &(Terrain[xValue][yValue]);
    }
    return nullptr;
}
void CellStorage::printCurrentStates()
{
    std::ofstream outFile("LogOfCurrentStates.txt");

    if (!outFile.is_open())
    {
        std::cout << "Smth goes wrong in writing in file" << std::endl;
    }

    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            auto a = std::to_string(static_cast<int>(Terrain[i][j].getState()));
            outFile << a;
        }
        outFile << '\n';
    };
    outFile << std::endl;
    outFile.close();
}