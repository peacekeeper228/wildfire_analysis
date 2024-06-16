#include "../include/Connection.h"
#include "../include/Properties.h"

Connection::Connection(CellStorage& storage)
:storage(storage)
{
    //auto conninfo = conninfo();
    char *conninfo = "dbname=your_db_name user=your_user_name password=your_password host=localhost port=5432";
    PGconn *conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        // If not successful, print the error message and finish the connection
        printf("Error while connecting to the database server: %s\n", PQerrorMessage(conn));

        // Finish the connection
        PQfinish(conn);

        // Exit the program
        exit(1);
        
    }
}

void Connection::setStatesToStorage()
    
{

    for (size_t i = 0; i < getXArea(); i++)
    {
        for (size_t j = 0; j < getYArea(); j++)
        {
            this->storage.setNewState(cellState::Tree, i, j);
            if (i == 6 && j > 10 && j < 100)
            {
                this->storage.setNewState(cellState::Water, i, j);
            };
            if (i == 50 && j > 100 && j < 1000)
            {
                this->storage.setNewState(cellState::Road, i, j);
            };
        };
    };
    for (size_t i = 0; i < numberOfStartingFirepoints(); i++)
    {
        this->storage.setNewState(cellState::Fire, (rand() % getXArea()), (rand() % getYArea()));
    };
};

Connection::~Connection()
{
}