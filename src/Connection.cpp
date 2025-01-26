#include "../include/Connection.h"
#include "../include/Properties.h"

Connection::Connection(CellStorage& storage)
:storage(storage)
{
    char *conninfo = "dbname=postgis user=postgis password=postgis host=localhost port=5431";
    connection = PQconnectdb(conninfo);
    if (PQstatus(connection) != CONNECTION_OK) {
        printf("Error while connecting to the database server: %s\n", PQerrorMessage(connection));
        PQfinish(connection);
        exit(1);
    };
    printf("Connection Established\n");
}

void Connection::setStatesToStorage()   
{
    char *query = "\
    SELECT x, y, val\
    FROM(\
        SELECT\
            q.*\
        FROM\
            dem r,\
            LATERAL ST_PixelAsCentroids(ST_Clip(r.rast, ST_GeomFromText('POLYGON((36.3 54.0, 36.4 54.0, 36.4 54.1, 36.3 54.1, 36.3 54.0))', 4326)), 1) as q\
        WHERE \
            ST_Intersects(r.rast, ST_GeomFromText('POLYGON((36.38 54.04, 36.381 54.04, 36.381 54.05, 36.38 54.05, 36.38 54.04))', 4326))\
            ) as t\
    WHERE val > 0;";
    // paramValues[0] = "1.1";
    // paramValues[0] = "1.2";
    // PGresult *res = PQexecParams(connection, query, 1, paramValues, NULL, NULL, NULL, 0);
    printf("Query started: %d\n", 1);
    PGresult *res = PQexec(connection, query);
    ExecStatusType resStatus = PQresultStatus(res);
    printf("Query Status: %s\n", PQresStatus(resStatus));
    if (resStatus != PGRES_TUPLES_OK) {
        printf("Error while executing the query: %s\n", PQerrorMessage(connection));
        PQclear(res);
        exit(1);
    }
    printf("%d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++) {
        char* a = PQgetvalue(res, i, 1); //"152"
        char* b = PQgetvalue(res, i, 0); //"POLYGON((36.38013888888888 54.04986111111112,36.38 54.04986111111112,36.38 54.05,36.38013888888888 54.05,36.38013888888888 54.04986111111112))"
        auto coordPairs = this->storage.getRelativePosition();
        auto cell = this->storage.checkAndGetCell(atoi(PQgetvalue(res, i, 0)), atoi(PQgetvalue(res, i, 1)));
        this->storage.setAltitudeToCell(
            atoi(PQgetvalue(res, i, 0)),
            atoi(PQgetvalue(res, i, 1)),
            atoi(PQgetvalue(res, i, 2))
        );
        // if (PQgetvalue(res, i, 1) > 0){
        //     this->storage.setNewState(cellState::Water, coordPairs.first, coordPairs.second);
        // }
    }

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
    query = "\
    SELECT brightness, ST_X(geom), ST_Y(geom)\
    FROM FIRE f\
    WHERE ST_Intersects(f.geom, ST_GeomFromText('POLYGON((10.0 30.0, 50.0 30.0, 50.0 54.04, 10.0 54.05, 10.0 30.0))', 4326))";
    res = PQexec(connection, query);
    resStatus = PQresultStatus(res);
    printf("Query Status: %s\n", PQresStatus(resStatus));
    if (resStatus != PGRES_TUPLES_OK) {
        printf("Error while executing the query: %s\n", PQerrorMessage(connection));
        PQclear(res);
        exit(1);
    }
    printf("%d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++) {
        double_t k = atof(PQgetvalue(res, i, 1));
        this->storage.setNewState(cellState::Fire,
            (atof(PQgetvalue(res, i, 1))-this->storage.latitudeMin)*10,
            (atof(PQgetvalue(res, i, 2))-this->storage.longtitudeMin)*10
            );
    };
};

Connection::~Connection()
{
    // PQclear(result);
    // PQfinish(conn);
    PQfinish(connection);
}