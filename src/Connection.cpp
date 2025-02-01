#include <format>
#include "../include/Connection.h"
#include "../include/Properties.h"
#include "Connection.h"

Connection::Connection(CellStorage &storage)
    : storage(storage)
{
    std::string conninfo = "dbname=postgis user=postgis password=postgis host=localhost port=5431";
    connection = PQconnectdb(conninfo.c_str());
    if (PQstatus(connection) != CONNECTION_OK)
    {
        printf("Error while connecting to the database server: %s\n", PQerrorMessage(connection));
        PQfinish(connection);
        exit(1);
    };
    printf("Connection Established\n");
}

PGresult* Connection::run_query(const char * formatted_dem_query) const
{
    PGresult *res = PQexec(connection, formatted_dem_query);
    ExecStatusType resStatus = PQresultStatus(res);
    printf("Query Status: %s\n", PQresStatus(resStatus));
    if (resStatus != PGRES_TUPLES_OK)
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(connection));
        PQclear(res);
        exit(1);
    }
    return res;
}

void Connection::setStatesToStorage()
{
    std::string polygon = "ST_GeomFromText('POLYGON((36.7 55.3, 36.75 55.3, 36.75 55.35, 36.7 55.35, 36.7 55.3))', 4326)";
    
    const std::string formatted_dem_query = std::format("\
    SELECT x, y, val\
    FROM(\
        SELECT\
            q.*\
        FROM\
            dem r,\
            LATERAL ST_PixelAsCentroids(ST_Clip(r.rast, {}), 1) as q\
        WHERE \
            ST_Intersects(r.rast, {})\
            ) as t\
    WHERE val > 0;", polygon, polygon);

    printf("Query started: %d\n", 1);
    PGresult *res = this->run_query(formatted_dem_query.c_str());
    
    printf("dem %d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++)
    {
        auto coordPairs = this->storage.getRelativePosition();
        auto cell = this->storage.checkAndGetCell(atoi(PQgetvalue(res, i, 0)), atoi(PQgetvalue(res, i, 1)));
        this->storage.setAltitudeToCell(
            atoi(PQgetvalue(res, i, 0)),
            atoi(PQgetvalue(res, i, 1)),
            atoi(PQgetvalue(res, i, 2)));
    }

    const std::string formatted_biomass_query = std::format("\
    SELECT x, y, val\
    FROM(\
        SELECT\
            q.*\
        FROM\
            biomass r,\
            LATERAL ST_PixelAsCentroids(ST_Clip(r.rast, {}), 1) as q\
        WHERE \
            ST_Intersects(r.rast, {})\
            ) as t\
    WHERE val > 0;", polygon, polygon);
    res = this->run_query(formatted_biomass_query.c_str());
    printf("biomass %d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++)
    {
        double_t k = atof(PQgetvalue(res, i, 1));
        this->storage.setNewState(cellState::Tree,
                                  atoi(PQgetvalue(res, i, 0)),
                                  atoi(PQgetvalue(res, i, 1)));
    };

    const std::string formatted_fire_query = std::format("\
    SELECT brightness, ST_X(geom), ST_Y(geom)\
    FROM FIRE f\
    WHERE ST_Intersects(f.geom, {})", polygon);
    res = this->run_query(formatted_fire_query.c_str());
    printf("%d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++)
    {
        double_t k = atof(PQgetvalue(res, i, 1));
        this->storage.setNewState(cellState::Fire,
                                  (atof(PQgetvalue(res, i, 1)) - this->storage.latitudeMin) * 10,
                                  (atof(PQgetvalue(res, i, 2)) - this->storage.longtitudeMin) * 10);
    };
};

Connection::~Connection()
{
    // PQclear(result);
    // PQfinish(conn);
    PQfinish(connection);
}