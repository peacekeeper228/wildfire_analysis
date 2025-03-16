#include "../include/Fire.h"

Fire::Fire()
    : fire_lifetime_(0), state_(FireState::Cursory)
{
}

FireState Fire::getState() const
{
    return this->state_;
}
void Fire::iterate()
{
    fire_lifetime_++;
    if (fire_lifetime_ == 1)
    {
        this->state_ = FireState::FullyDeveloped;
    }
    else if (fire_lifetime_ == 4)
    {
        this->state_ = FireState::Declining;
    }
    else if (fire_lifetime_ == 5)
    {
        this->state_ = FireState::Ended;
    };
    return;
}
bool Fire::fireEnded() const
{
    return fire_lifetime_ >= 5;
}

bool Fire::canSpread() const
{
    return this->state_ == FireState::FullyDeveloped;
}
Fire::~Fire()
{
}