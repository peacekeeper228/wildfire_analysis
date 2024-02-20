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
    bool fireEnded() const;
    ~Fire();
};

