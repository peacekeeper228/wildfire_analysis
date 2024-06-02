#pragma once
#include <libpq-fe.h>
#include "CellStorage.h"

class Connection
{
private:
    int connection;
    CellStorage& storage;

public:
    Connection(CellStorage& storage);
    void setStatesToStorage();
    ~Connection();
};
