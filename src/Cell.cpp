#include "../include/Cell.h"

cell::cell()
    : futureState(cellState::NoState), windState(nullptr), fireState(nullptr)
{
    this->currentState = cellState::Empty;
}

void setWindToCell(cell *changingCell, std::shared_ptr<const Wind> assigningWind)
{
    changingCell->setWind(assigningWind);
}

void cell::setWind(std::shared_ptr<const Wind> wind)
{
    this->windState = wind;
}

std::shared_ptr<const Wind> cell::getWind() const
{
    return this->windState;
}
cellState cell::getState() const
{
    return this->currentState;
}

void cell::setState(const cellState &state)
{
    if (state == cellState::Fire)
    {
        inFire();
    }
    else
    {
        this->currentState = state;
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
    // cause for now only in fire there can be dynamic changes. Forest is not growing))))
    if (!(currentState == cellState::Fire))
    {
        return;
    };
    fireState->iterate();
    if (fireState->fireEnded())
    {
        fireState->~Fire();
        futureState = cellState::Burnt;
    };
}

void cell::setNewState()
{
    if ((futureState != cellState::NoState) & (futureState != currentState))
    {
        currentState = futureState;
        futureState = cellState::NoState;
    }
}

Fire *cell::getFireInCell() const
{
    return this->fireState;
}

cell::~cell()
{
    setWindToCell(this, nullptr);
    this->getFireInCell()->~Fire();
}
