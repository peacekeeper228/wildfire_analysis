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
    
    /// @brief basic function to make iteration and calls iterations of all childs
    void iterate();

    /// @brief return the flag lifetime has ended
    /// @return true if all logic has been proceeded
    bool fireEnded() const;

    bool canSpread() const;
    ~Fire();
};

