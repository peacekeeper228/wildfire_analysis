#pragma once
#include <libpq-fe.h>
#include "CellStorage.h"

class Connection
{
private:
    PGconn* connection;
    CellStorage& storage;

    PGresult* run_query(const char*) const;
public:
    Connection(CellStorage& storage);
    void setStatesToStorage();
    ~Connection();
};
