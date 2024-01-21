#include "../include/Cell.h"

cell::cell()
{
    this->currentState = cellState::Empty;
    this->windState = nullptr;
}

void cell::setWind(const Wind* wind)
{
    this->windState = wind;
}

Wind *cell::getWind() const
{
    this->windState;
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

cell::~cell()
{
}
