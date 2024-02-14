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
    
    void inFire();
    void iterate();
    void setNewState();
    ~cell();
};
void setWindToCell(cell* c, Wind* w);
