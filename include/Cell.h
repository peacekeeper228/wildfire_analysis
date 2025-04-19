#pragma once
#include "CellState.h"
#include "Wind.h"
#include "Fire.h"
#include <memory>
class cell
{
private:
    cellState future_state_;
    cellState current_state_;
    std::shared_ptr<const Wind> wind_state_;
    std::unique_ptr<Fire> fire_state_;
    int biomass_;
    double altitude_;
    
public:
    cell();
    cell(cell&& other);
    cell(const cell& other) = delete;
    cell& operator=(cell &&other);

    void setWind(std::shared_ptr<const Wind> wind);
    std::shared_ptr<const Wind> getWind() const;

    /// @brief set state directly to the cell (ignite if fire)
    /// @param state state that will be set
    void setState(const cellState& state);
    cellState getState() const;
    
    /// @brief carefully set state to fire with checking the possibility to do this
    void ignite();

    /// @brief basic function to make iteration and call iterations of all childs
    void iterate();

    /// @brief possible state becomes present, often is called during iteration
    void setNewState();

    void setAltitude(int altitude);
    int getAltitude() const;
    ~cell();

    void setBiomass(const int biomass);
    int getBiomass() const;

    const Fire* getFireInCell() const;
};
/// @brief this function make a proper assignment wind to cell. Also it follows invariants in wind
/// @param changingCell cell, wind is assigned to this one
/// @param assigningWind new wind that is assigned to cell
void setWindToCell(cell* changingCell, std::shared_ptr<const Wind> assigningWind);