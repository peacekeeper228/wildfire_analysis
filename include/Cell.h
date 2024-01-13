#ifndef CELL_H
#define CELL_H
#include "CellState.h"
class cell
{
private:
    cellState currentState;
    
public:
    cell();

    cellState getState() const;
    void setState(const cellState& State);
    
    void inFire();

    ~cell();
};
#endif