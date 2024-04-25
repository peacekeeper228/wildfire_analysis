#pragma once

#include <cstddef>
#include <list>
#include <math.h>
#include <utility>
#include <vector>

#include "Wind.h"
#include "Properties.h"
#include "Cell.h"

inline double NumberOfCellAffectedInParticularDirection(double koeff, float fireSpeed)
{
    return minutesPerIteration() * (double)fireSpeed * koeff / cellSizeInMeters();
}

inline double calculateKoeff(float windSpeed, double slopeAngleRad)
{
    // currently, I suppose this is more example than real formula :(
    int windKoeff = pow(196, 0.0133 * windSpeed);
    int slopeKoeff = 5.275 * tan(slopeAngleRad);
    return windKoeff * slopeKoeff;
}
class CellStorage
{
private:
    size_t time_after;
    std::vector<std::vector<std::shared_ptr<cell>>> Terrain;
    //cell Terrain[getXArea()][getYArea()];

public:
    CellStorage();
    ~CellStorage();

    /// @brief set state of a particular cell with no checking bound invariant
    /// @param state state that is set
    /// @param xValue row of cell in cell storage
    /// @param yValue column of cell in cell storage
    void setNewState(const cellState &state, int xValue, int yValue);

    /// @brief get state of a particular cell
    /// @param xValue row of cell in cell storage
    /// @param yValue column of cell in cell storage
    /// @return cell state
    cellState getState(int xValue, int yValue) const;

    /// @brief method to safely get cell and check that x,y are in bound
    /// @param xValue number of row where needed cell is placed
    /// @param yValue number of column where needed cell is placed
    /// @return reference to cell in storage or nullptr
    const cell *checkAndGetCell(int xValue, int yValue) const;

    /// @brief basic function to make iteration and calls iterations of all childs
    void iterate();

    void iterateCell(int i, int j);

    /// @brief link a number of cells to a particular wind instance
    /// Attention! If any range invariant is not proper, no assignment is made
    /// @param xRange range of rows
    /// @param yRange range of columns
    /// @param w wind, that is assigned to the group of cells
    /// @return sign of success
    bool setWindToArea(const std::pair<int, int> xRange, const std::pair<int, int> yRange,  std::shared_ptr<const Wind> w);

    /// @brief all current states of cell storage is stored to 'LogOfCurrentStates.txt' in root project directory
    void printCurrentStates();
};
