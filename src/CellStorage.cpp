#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
// #include <json.h>

#include "../include/CellStorage.h"
#include "../include/Coordinates.h"

CellStorage::CellStorage(Math *formula)
    : formula(formula)
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
}

CellStorage::~CellStorage()
{
}

void CellStorage::iterateSquare(int xMin, int yMin, int xMax, int yMax)
{
    for (int i = xMin; i < xMax; i++)
    {
        for (int j = yMin; j < yMax; j++)
        {
            this->iterateCell(i, j);
        }
    };
}

void CellStorage::iterate()
{
    std::thread thread1(&CellStorage::iterateSquare, this, 0, 0, getXArea() / 2, getYArea() / 2);
    std::thread thread2(&CellStorage::iterateSquare, this, 0, getYArea() / 2, getXArea() / 2, getYArea());
    std::thread thread3(&CellStorage::iterateSquare, this, getXArea() / 2, 0, getXArea(), getYArea() / 2);
    std::thread thread4(&CellStorage::iterateSquare, this, getXArea() / 2, getYArea() / 2, getXArea(), getYArea());
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    // take 55 % of time iteration
    // for (int i = 0; i < getXArea(); i++)
    // {
    //     for (int j = 0; j < getYArea(); j++)
    //     {
    //         this->iterateCell(i, j);
    //     }
    // };
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

void CellStorage::iterate(int n, int xMin, int yMin, int xMax, int yMax)
{
}

void CellStorage::iterateCell(int i, int j)
{
    if (getState(i, j) != cellState::Fire)
    {
        return;
    };
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
const cell *CellStorage::checkAndGetCell(int xValue, int yValue) const
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

void CellStorage::saveFiresToJson()
{
    //     Json::Value event;
    //     event["version"] = "0.0.1";
    //     Json::Value data(Json::arrayValue);
    //     for (size_t i = 0; i < getXArea(); i++)
    //     {
    //         for (size_t j = 0; j < getYArea(); j++)
    //         {
    //             auto stateOfCurrentCell = Terrain[i][j]->getState();
    //             if (stateOfCurrentCell == cellState::Burnt || stateOfCurrentCell == cellState::Fire){
    //                 Json::Value vec(Json::arrayValue);
    //                 vec.append(Json::Value(i));
    //                 vec.append(Json::Value(j));
    //                 vec.append(Json::Value(static_cast<int>(stateOfCurrentCell)));
    //                 data.append(vec);
    //             }
    //         }
    //     }

    //     event["data"] = data;
    //     std::cout << event << std::endl;
}

std::vector<std::pair<int, int>> CellStorage::getRelativeFirePoints() const
{
    // important note that result must be sorted!
    std::vector<std::pair<int, int>> result;
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            if (Terrain[i][j]->getState() == cellState::Fire){
                result.push_back(std::pair(i, j));
            }
        }
    }
    return result;
}

std::vector<std::pair<int, int>> CellStorage::getBurntPoints() const
{
    // important note that result must be sorted!
    std::vector<std::pair<int, int>> result;
    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            if (Terrain[i][j]->getState() == cellState::Burnt){
                result.push_back(std::pair(i, j));
            }
        }
    }
    return result;
}