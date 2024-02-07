#pragma once
#include <cstddef>

enum class FireState
{
    Cursory,
    FullyDeveloped,
    Declining,
    Ended
};

class Fire
{
private:
    std::size_t fireLifetime;
    FireState state;
public:
    Fire();
    FireState getState() const;
    void iterate();
    bool fireEnded() const;
    ~Fire();
};

