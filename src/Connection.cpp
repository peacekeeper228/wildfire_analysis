#include "../include/Connection.h"
#include "../include/Properties.h"

Connection::Connection(CellStorage& storage)
:storage(storage)
{
    char *conninfo = "dbname=postgis user=postgis password=postgis host=postgisDB port=5432";
    // connection = PQconnectdb(conninfo);
    // if (PQstatus(connection) != CONNECTION_OK) {
    //     printf("Error while connecting to the database server: %s\n", PQerrorMessage(connection));
    //     PQfinish(connection);
    //     exit(1);
    // };
    // printf("Connection Established\n");
}

void Connection::setStatesToStorage()   
{
    // char *query = "\
    //     SELECT ST_Value(r.rast, g.geom)\
    //     FROM world AS r\
    //     INNER JOIN\
	// 	ST_Point(36.38766,56.04554, 4326) AS g(geom)\
    //         ON ST_Intersects(r.rast, g.geom)";
    // char buffer[64];
    // const char *paramValues[1];
    // paramValues[0] = "1.1";
    // paramValues[0] = "1.2";

    // PGresult *res = PQexecParams(connection, query, 1, NULL, paramValues, NULL, NULL, 0);
    // ExecStatusType resStatus = PQresultStatus(res);
    // printf("Query Status: %s\n", PQresStatus(resStatus));
    // if (resStatus != PGRES_TUPLES_OK) {
    //     printf("Error while executing the query: %s\n", PQerrorMessage(connection));
    //     PQclear(res);
    //     exit(1);
    // }
    // for (int i = 0; i < PQntuples(res); i++) {
    //     printf("%s\t", PQgetvalue(res, i, 2));
    //     auto coordPairs = this->storage.getRelativePosition();
    //     if (PQgetvalue(res, i, 1) == 0){
    //         this->storage.setNewState(cellState::Water, coordPairs.first, coordPairs.second);
    //     }
    // }

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
    // PQfinish(connection);
}