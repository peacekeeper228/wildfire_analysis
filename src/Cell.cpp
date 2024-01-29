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

const Wind* cell::getWind() const
{   
    return this->windState;
}
cellState cell::getState() const
{
    return this->currentState;
}

void cell::setState(const cellState& State)
{   
    if (State == cellState::Fire)
    {
        inFire();
    }
    
    this->currentState = State;
}
void cell::inFire()
{
    if (currentState == cellState::Tree)
    {
        currentState = cellState::Fire;
        fireLifeTime = 10;
    }
}

void cell::iterate()
{
    if (!(currentState == cellState::Fire)){
        return;
    };
    fireLifeTime--;
    if (fireLifeTime == 0){
        this->setState(cellState::Burnt);
    };
}
cell::~cell()
{
}
