#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
// #include <json.h>

#include "../include/CellStorage.h"
#include "../include/Coordinates.h"

CellStorage::CellStorage(Math *formula)
: CellStorage(formula, getXArea(),getYArea())
{
}

CellStorage::CellStorage(Math *formula, int16_t x_size, int16_t y_size)
    : formula_(formula), x_size_(x_size), y_size_(y_size)
{
    time_after_ = 0;
    terrain_.resize(x_size_, std::vector<std::shared_ptr<cell>>(y_size_));
    for (size_t i = 0; i < x_size_; i++)
    {
        for (size_t j = 0; j < y_size_; j++)
        {
            terrain_[i][j] = std::make_shared<cell>();
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
    std::thread thread1(&CellStorage::iterateSquare, this, 0, 0, x_size_ / 2, y_size_ / 2);
    std::thread thread2(&CellStorage::iterateSquare, this, 0, y_size_ / 2, x_size_ / 2, y_size_);
    std::thread thread3(&CellStorage::iterateSquare, this, x_size_ / 2, 0, x_size_, y_size_ / 2);
    std::thread thread4(&CellStorage::iterateSquare, this, x_size_ / 2, y_size_ / 2, x_size_, y_size_);
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    // take 55 % of time iteration
    // for (int i = 0; i < x_size_; i++)
    // {
    //     for (int j = 0; j < y_size_; j++)
    //     {
    //         this->iterateCell(i, j);
    //     }
    // };
    // take 45 % of time iteration
    for (size_t i = 0; i < x_size_; i++)
    {
        for (size_t j = 0; j < y_size_; j++)
        {
            terrain_[i][j]->iterate();
        }
    };
    time_after_++;
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
    const cell *iteratedCell = checkAndGetCell(i, j);
    // we can do that because invariant is checked in cell
    if (iteratedCell->getFireInCell()->canSpread())
    {
        auto listKoef = std::list<double>();
        for (auto analyzedDirection : getAllDirections())
        {
            auto x = getShiftingOnDirections(analyzedDirection);
            auto nearestCell = this->checkAndGetCell(i + x.first, j + x.second);
            if (nearestCell == nullptr)
            {
                continue;
            };
            double fireKoeff = 0;
            auto state_nearest_cell = nearestCell->getState();
            if (state_nearest_cell == cellState::Tree)
            {
                if (formula_->willSpread(iteratedCell, analyzedDirection, iteratedCell->getAltitude() - nearestCell->getAltitude()))
                {
                    setNewState(cellState::Fire, i + x.first, j + x.second);
                };
            };
            auto throughCell = this->checkAndGetCell(i + 2 * x.first, j + 2 * x.second);
            if (throughCell == nullptr)
            {
                continue;
            };
            auto state_through_cell = throughCell->getState();
            if ((state_through_cell == cellState::Tree) && (state_nearest_cell != cellState::Tree) && ((state_nearest_cell != cellState::Fire)))
            {
                if (formula_->willSpreadThroughOne(iteratedCell, analyzedDirection, iteratedCell->getAltitude() - throughCell->getAltitude()))
                {
                    setNewState(cellState::Fire, i + 2 * x.first, j + 2 * x.second);
                };
            };
        };
    };
};

bool CellStorage::setWindToArea(const std::pair<int, int> xRange, const std::pair<int, int> yRange, std::shared_ptr<const Wind> w)
{
    if ((xRange.first < 0) | (xRange.second > x_size_) | (yRange.first < 0) | (yRange.second > y_size_))
    {
        return false;
        // TODO: что-то надо делать
    };

    for (int i = xRange.first; i < xRange.second; i++)
    {
        for (int j = yRange.first; j < yRange.second; j++)
        {
            setWindToCell((terrain_[i][j].get()), w);
        }
    }
    return true;
}
void CellStorage::setNewState(const cellState &state, int xValue, int yValue)
{  
    terrain_[xValue][yValue]->setState(state);
}

cellState CellStorage::getState(int xValue, int yValue) const
{
    return terrain_[xValue][yValue]->getState();
}
const cell *CellStorage::checkAndGetCell(int xValue, int yValue) const
{
    if (xValue >= 0 && xValue < x_size_ && yValue >= 0 && yValue < y_size_)
    {
        return (terrain_[xValue][yValue].get());
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

    for (int16_t i = 0; i < x_size_; i++)
    {
        for (int16_t j = 0; j < y_size_; j++)
        {
            auto a = std::to_string(static_cast<int>(terrain_[i][j]->getState()));
            outFile << a;
        }
        outFile << '\n';
    };
    outFile << std::endl;
    outFile.close();
}

void CellStorage::setAltitudeToCell(int x, int y, int value)
{
    terrain_[x][y].get()->setAltitude(value);
}
void CellStorage::saveFiresToJson()
{
    //     Json::Value event;
    //     event["version"] = "0.0.1";
    //     Json::Value data(Json::arrayValue);
    //     for (size_t i = 0; i < x_size_; i++)
    //     {
    //         for (size_t j = 0; j < y_size_; j++)
    //         {
    //             auto stateOfCurrentCell = terrain_[i][j]->getState();
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

void CellStorage::uploadFromTxt()
{
    std::ifstream txt_file{"previous.txt"};
    if (!txt_file.is_open()) {
        std::cerr << "Не удалось открыть файл с предыдущими данными" << std::endl;
        return;
    }
    std::string line;
    size_t currentLine = 0;

    while (std::getline(txt_file, line)) {
        for (size_t i = 0; i < line.size(); i++)
        {
            cellState new_state = static_cast<cellState>(line[i]-'0');
            if (new_state == cellState::Fire){
                setNewState(cellState::Tree, currentLine, i);
            }
            setNewState(new_state, currentLine, i);
        }
        currentLine++;
        
    }
    txt_file.close();
}
std::vector<std::pair<int, int>> CellStorage::getRelativeFirePoints() const
{
    // important note that result must be sorted!
    std::vector<std::pair<int, int>> result;
    for (size_t i = 0; i < x_size_; i++)
    {
        for (size_t j = 0; j < y_size_; j++)
        {
            if (terrain_[i][j]->getState() == cellState::Fire)
            {
                result.push_back(std::pair(i, j));
            }
        }
    }
    return result;
}

std::vector<std::pair<int, int>> CellStorage::getRelativeBurntPoints() const
{
    // important note that result must be sorted!
    std::vector<std::pair<int, int>> result;
    for (size_t i = 0; i < x_size_; i++)
    {
        for (size_t j = 0; j < y_size_; j++)
        {
            if (terrain_[i][j]->getState() == cellState::Burnt)
            {
                result.push_back(std::pair(i, j));
            }
        }
    }
    return result;
}

CellStorage* CellStorage::CopySquare(int xMin, int yMin, int xMax, int yMax) const
{
    auto c = new CellStorage(this->formula_, xMax-xMin, yMax-yMin);
    return c;
}

std::pair<int, int> CellStorage::getRelativePosition() const
{
    return std::make_pair<int, int>(0,0);
}