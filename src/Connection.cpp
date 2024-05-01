#include "../include/Connection.h"
#include "../include/Properties.h"

Connection::Connection(CellStorage& storage)
:storage(storage)
{
}

void Connection::setStatesToStorage()
    
{

    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            this->storage.setNewState(cellState::Tree, i, j);
            if (i == 6)
            {
                this->storage.setNewState(cellState::Water, i, j);
            }
        }
    }
};

Connection::~Connection()
{
}