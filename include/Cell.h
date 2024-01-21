#pragma once
#include "CellState.h"
#include "Wind.h"
class cell
{
private:
    cellState currentState;
    const Wind* windState;
    
public:
    cell();

    void setWind(const Wind* wind);
    Wind* getWind() const;

    cellState getState() const;
    void setState(const cellState& State);
    
    void inFire();

    ~cell();
};
