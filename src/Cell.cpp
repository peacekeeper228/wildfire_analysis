#include "../include/Cell.h"

cell::cell()
{
    this->currentState = cellState::Empty;
}

cell::~cell()
{
}

cellState cell::getState() const
{
    return this->currentState;
}

void cell::setState(const cellState& State)
{
    this->currentState = State;
}
void cell::inFire()
{
    if (currentState == cellState::Tree)
    {
        currentState = cellState::Fire;
    }
}

