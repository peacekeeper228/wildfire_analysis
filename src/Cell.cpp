#include "../include/Cell.h"

cell::cell()
{
    this->currentState = cellState::Empty;
    this->windState = nullptr;
    this->futureState = cellState::NoState;
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
    } else {
        this->currentState = State;
    };
}
void cell::inFire()
{
    if (currentState == cellState::Tree)
    {
        futureState = cellState::Fire;
        this->fireState = new Fire();
    }
}

void cell::iterate()
{
    if (!(currentState == cellState::Fire)){
        return;
    };
    fireState->iterate();
    if (fireState->fireEnded()){
        fireState->~Fire();
        //currentState = cellState::Burnt;
        futureState = cellState::Burnt;
        //this->setState(cellState::Burnt);
    };
}

void cell::setNewState()
{
    if ((futureState != cellState::NoState) & (futureState != currentState)){
        currentState = futureState;
        futureState = cellState::NoState;
    } 
}
cell::~cell()
{
}
