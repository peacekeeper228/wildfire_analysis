#pragma once
#include "CellState.h"
#include "Wind.h"
#include "Fire.h"
class cell
{
private:
    cellState currentState;
    const Wind* windState;
    Fire* fireState;
    
public:
    cell();

    void setWind(const Wind* wind);
    const Wind* getWind() const;

    cellState getState() const;
    void setState(const cellState& State);
    
    void inFire();
    void iterate();
    ~cell();
};
