#pragma once
#include "CellState.h"
#include "Wind.h"
#include "Fire.h"
class cell
{
private:
    cellState currentState;
    cellState futureState;
    const Wind* windState;
    Fire* fireState;
    
public:
    cell();

    void setWind(const Wind* wind);
    const Wind* getWind() const;

    cellState getState() const;
    void setState(const cellState& State);
    
    /// @brief carefully set state to fire with checking the possibility to do this
    void inFire();

    /// @brief basic function to make iteration and calls iterations of all childs
    void iterate();

    /// @brief possible state becomes present, often is calles during iteration
    void setNewState();
    ~cell();

    Fire* getFireInCell() const;
};
/// @brief this function make a proper assignment wind to cell. Also it follows invariants in wind
/// @param c cell, wind is assigned to this cell
/// @param w new wind that is assigned to cell
void setWindToCell(cell* c, Wind* w);