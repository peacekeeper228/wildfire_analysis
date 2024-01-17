#ifndef CELLSTORAGE_H
#define CELLSTORAGE_H

#include "Cell.h"
#include <cstddef>
#include <list>
#include <math.h>

constexpr int getXArea(){
    return 10;
}

constexpr int getYArea(){
    return 10;
}

constexpr int ignitionPercentage(){
    return 50;
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
    //currently, I suppose this is more example formulas :(
    int windKoeff = pow(196, 0.0133 * windSpeed);
    int slopeKoeff = 5.275 * tan(slopeAngleRad);
    return windKoeff * slopeKoeff;
}

class CellStorage
{
private:
    size_t time_after;
    cell Terrain[getXArea()][getYArea()]; 
public:
    void setNewState(const cellState& state, int xValue, int yValue);
    cellState getState(int xValue, int yValue) const;
    const cell* checkAndGetCell(int xValue, int yValue) const;
    CellStorage();
    ~CellStorage();
    void iterate();
    std::list<const cell*> getNeighbors(int xValue, int yValue) const;
};

#endif