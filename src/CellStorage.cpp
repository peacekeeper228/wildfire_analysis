#include "../include/CellStorage.h"
#include "../include/Coordinates.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

CellStorage::CellStorage()
{
    time_after = 0;
    Terrain.resize(getXArea(), std::vector<std::shared_ptr<cell>>(getYArea()));
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            Terrain[i][j] = std::make_shared<cell>();
        }
    }
    // vtr.resize(getXArea()*100, std::vector<cell>(getYArea()));
}

CellStorage::~CellStorage()
{
}

void CellStorage::iterate()
{
    // take 55 % of time iteration
    for (int i = 0; i < getXArea(); i++)
    {
        for (int j = 0; j < getYArea(); j++)
        {
            this->iterateCell(i, j);
        }
    };
    // take 45 % of time iteration
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            Terrain[i][j]->iterate();
        }
    };
    time_after++;
}

void CellStorage::iterateCell(int i, int j){
    if (getState(i, j) == cellState::Fire)
            {
                // we can do that because invariant is checked in cell
                if (checkAndGetCell(i, j)->getFireInCell()->canSpread())
                {
                    auto listKoef = std::list<double>();
                    for (auto analyzedDirection : getAllDirections())
                    {
                        auto x = getShiftingOnDirections(analyzedDirection);
                        auto a = this->checkAndGetCell(i + x.first, j + x.second);
                        if ((a != nullptr) && (a->getState() == cellState::Tree))
                        {
                            double fireKoeff = 0;
                            if (a->getWind() != nullptr)
                            {
                                fireKoeff = a->getWind()->CalculateWindKoef(analyzedDirection);
                            }
                            // TODO calculate k properly
                            if (int(fireKoeff * 100) + (rand() % 100) > ignitionPercentage())
                            {
                                setNewState(cellState::Fire, i + x.first, j + x.second);
                            }
                        }
                    }
                }
            }
}


bool CellStorage::setWindToArea(const std::pair<int, int> xRange, const std::pair<int, int> yRange, std::shared_ptr<const Wind> w)
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
            setWindToCell((Terrain[i][j].get()), w);
        }
    }
    return true;
}
void CellStorage::setNewState(const cellState &state, int xValue, int yValue)
{
    Terrain[xValue][yValue]->setState(state);
}

cellState CellStorage::getState(int xValue, int yValue) const
{
    return Terrain[xValue][yValue]->getState();
}
const cell* CellStorage::checkAndGetCell(int xValue, int yValue) const
{
    if (xValue >= 0 && xValue < getXArea() && yValue >= 0 && yValue < getYArea())
    {
        return (Terrain[xValue][yValue].get());
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
            auto a = std::to_string(static_cast<int>(Terrain[i][j]->getState()));
            outFile << a;
        }
        outFile << '\n';
    };
    outFile << std::endl;
    outFile.close();
}