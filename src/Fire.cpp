#include "../include/Fire.h"

Fire::Fire()
{
    this->fireLifetime = 0;
    this->state = FireState::Cursory;
}

FireState Fire::getState() const
{
    return this->state;
}
void Fire::iterate()
{
    fireLifetime++;
    if (fireLifetime == 1){
        this->state = FireState::FullyDeveloped;
    }else if (fireLifetime == 4)
    {
        this->state = FireState::Declining;
    }else if (fireLifetime == 5)
    {
        this->state = FireState::Ended;
    };
    return;
}
bool Fire::fireEnded() const
{
    return fireLifetime>=5;
}
Fire::~Fire()
{
}