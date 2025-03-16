#pragma once
#include <libpq-fe.h>
#include "CellStorage.h"

class Connection
{
private:
    PGconn* connection_;
    ;

    PGresult* runQuery(const char*) const;
    void setDemToStorage(CellStorage& storage);
    void setBiomassToStorage(CellStorage& storage);
    void setFireToStorage(CellStorage& storage);
    
public:
    Connection();
    void setStatesToStorage(CellStorage& storage);
    std::pair<int, int> calculateStorageSize();
    ~Connection();
};
