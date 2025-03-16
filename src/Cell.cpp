#include "../include/Cell.h"

cell::cell()
    : future_state_(cellState::NoState), wind_state_(nullptr), fire_state_(nullptr)
{
    this->current_state_ = cellState::Empty;
}

cell::cell(cell &&other)
    : future_state_(cellState::NoState), current_state_(other.current_state_), wind_state_(other.wind_state_)
{
    this->fire_state_ = std::move(other.fire_state_);
}

cell &cell::operator=(cell &&other)
{
    this->current_state_ = other.current_state_;
    this->wind_state_ = other.wind_state_;
    this->fire_state_ = std::move(other.fire_state_);
    return *this;
}

void setWindToCell(cell *changingCell, std::shared_ptr<const Wind> assigningWind)
{
    changingCell->setWind(assigningWind);
}

void cell::setWind(std::shared_ptr<const Wind> wind)
{
    this->wind_state_ = wind;
}

std::shared_ptr<const Wind> cell::getWind() const
{
    return this->wind_state_;
}
cellState cell::getState() const
{
    return this->current_state_;
}

void cell::setState(const cellState &state)
{
    if (state == cellState::Fire)
    {
        ignite();
    }
    else
    {
        this->current_state_ = state;
    };
}
void cell::ignite()
{
    if (current_state_ == cellState::Tree)
    {
        future_state_ = cellState::Fire;
        if (this->fire_state_ == nullptr)
        {
            this->fire_state_ = std::make_unique<Fire>();
        }
    }
}

void cell::iterate()
{

    // cause for now only in fire there can be dynamic changes. Forest is not growing))))
    if ((current_state_ == cellState::Fire))
    {

        fire_state_->iterate();
        if (fire_state_->fireEnded())
        {
            fire_state_ = nullptr;
            future_state_ = cellState::Burnt;
        };
    };

    setNewState();
}

void cell::setNewState()
{
    if ((future_state_ != cellState::NoState) & (future_state_ != current_state_))
    {
        current_state_ = future_state_;
        future_state_ = cellState::NoState;
        if (current_state_ == cellState::Fire)
        {
            ignite();
        }
    }
}

const Fire *cell::getFireInCell() const
{
    return this->fire_state_.get();
}

void cell::setAltitude(int altitude)
{
    this->altitude_ = altitude;
}

int cell::getAltitude() const
{
    return this->altitude_;
}
cell::~cell()
{
    setWindToCell(this, nullptr);
}
