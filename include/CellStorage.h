#pragma once
#include "Cell.h"
#include <cstddef>
#include <list>
#include <math.h>
#include <utility>

#include "Wind.h"
#include <vector>

constexpr int getXArea(){
    return 1000;
}

constexpr int getYArea(){
    return 1000;
}

constexpr int ignitionPercentage(){
    return 150; // in formula there is smth wrong I suppose
}

constexpr int cellSizeInMeters(){
    return 20;
}

inline int minutesPerIteration(){
    return 10;
}

inline double NumberOfCellAffectedInParticularDirection(double koeff, float fireSpeed){
    return minutesPerIteration() * fireSpeed * koeff / cellSizeInMeters();
}

inline double calculateKoeff(float windSpeed, double slopeAngleRad){
    //currently, I suppose this is more example than real formula :(
    int windKoeff = pow(196, 0.0133 * windSpeed);
    int slopeKoeff = 5.275 * tan(slopeAngleRad);
    return windKoeff * slopeKoeff;
}
class CellStorage
{
private:
    size_t time_after;
    std::vector<std::vector<cell>> Terrain;
    //cell Terrain[getXArea()][getYArea()]; 
public:
    void setNewState(const cellState& state, int xValue, int yValue);
    cellState getState(int xValue, int yValue) const;
    const cell* checkAndGetCell(int xValue, int yValue) const;
    CellStorage();
    ~CellStorage();
    void iterate();
    std::list<double> getNeighborsKoeff(int xValue, int yValue) const;
    std::list<const cell*> getNeighbors(int xValue, int yValue) const;
    void setWindToArea(const std::pair<int, int> xRange, const std::pair<int, int> yRange, const Wind* w);
    void printCurrentStates();
};
