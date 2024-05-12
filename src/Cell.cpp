#include "../include/Cell.h"

cell::cell()
    : futureState(cellState::NoState), windState(nullptr), fireState(nullptr)
{
    this->currentState = cellState::Empty;
}

cell::cell(cell &&other)
    : futureState(cellState::NoState), currentState(other.currentState), windState(other.windState)
{
    this->fireState = std::move(other.fireState);
}

cell &cell::operator=(cell &&other)
{
    this->currentState = other.currentState;
    this->windState = other.windState;
    this->fireState = std::move(other.fireState);
    return *this;
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
        ignite();
    }
    else
    {
        this->currentState = state;
    };
}
void cell::ignite()
{
    if (currentState == cellState::Tree)
    {
        futureState = cellState::Fire;
        if (this->fireState == nullptr)
        {
            this->fireState = std::make_unique<Fire>();
        }
    }
}

void cell::iterate()
{

    // cause for now only in fire there can be dynamic changes. Forest is not growing))))
    if ((currentState == cellState::Fire))
    {

        fireState->iterate();
        if (fireState->fireEnded())
        {
            fireState = nullptr;
            futureState = cellState::Burnt;
        };
    };

    setNewState();
}

void cell::setNewState()
{
    if ((futureState != cellState::NoState) & (futureState != currentState))
    {
        currentState = futureState;
        futureState = cellState::NoState;
        if (currentState == cellState::Fire)
        {
            ignite();
        }
    }
}

const Fire *cell::getFireInCell() const
{
    return this->fireState.get();
}

cell::~cell()
{
    setWindToCell(this, nullptr);
}
