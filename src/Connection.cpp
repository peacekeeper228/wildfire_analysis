#include <format>
#include "../include/Connection.h"
#include "../include/Properties.h"

Connection::Connection()
{
    std::string conninfo = "dbname=postgis user=postgis password=postgis host=localhost port=5431";
    connection_ = PQconnectdb(conninfo.c_str());
    if (PQstatus(connection_) != CONNECTION_OK)
    {
        printf("Error while connecting to the database server: %s\n", PQerrorMessage(connection_));
        PQfinish(connection_);
        exit(1);
    };
    printf("Connection Established\n");
}

PGresult* Connection::runQuery(const char * formatted_dem_query) const
{
    PGresult *res = PQexec(connection_, formatted_dem_query);
    ExecStatusType resStatus = PQresultStatus(res);
    printf("Query Status: %s\n", PQresStatus(resStatus));
    if (resStatus != PGRES_TUPLES_OK)
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(connection_));
        PQclear(res);
        exit(1);
    }
    return res;
}

void Connection::setDemToStorage(CellStorage& storage)
{
    const std::string formatted_dem_query = std::format("\
    SELECT x-1, y-1, val\
    FROM(\
        SELECT\
            q.*\
        FROM\
            (SELECT ST_Rescale(rast, 0.0005, 0.0005) as rast from dem) r,\
            LATERAL ST_PixelAsCentroids(ST_Clip(r.rast, {}), 1) as q\
        WHERE \
            ST_Intersects(r.rast, {})\
            ) as t\
    WHERE val > 0;", analyzedPolygon(), analyzedPolygon());

    printf("Query started: %d\n", 1);
    PGresult *res = this->runQuery(formatted_dem_query.c_str());
    
    printf("dem %d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++)
    {
        auto coordPairs = storage.getRelativePosition();
        auto cell = storage.checkAndGetCell(atoi(PQgetvalue(res, i, 0)), atoi(PQgetvalue(res, i, 1)));
        storage.setAltitudeToCell(
            atoi(PQgetvalue(res, i, 0)),
            atoi(PQgetvalue(res, i, 1)),
            atoi(PQgetvalue(res, i, 2)));
    }
}

void Connection::setBiomassToStorage(CellStorage& storage)
{
    const std::string formatted_biomass_query = std::format("\
    SELECT x-1, y-1, val\
    FROM(\
        SELECT\
            q.*\
        FROM\
            (SELECT ST_Rescale(rast, 0.0005, 0.00025) as rast from biomass) r,\
            LATERAL ST_PixelAsCentroids(ST_Clip(r.rast, {}), 1) as q\
        WHERE \
            ST_Intersects(r.rast, {})\
            ) as t\
    WHERE val > 80 and y < {};", analyzedPolygon(), analyzedPolygon(), getYArea());
    PGresult *res = this->runQuery(formatted_biomass_query.c_str());
    printf("biomass %d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++)
    {
        double_t k = atof(PQgetvalue(res, i, 1));
        storage.setNewState(cellState::Tree,
                                  atoi(PQgetvalue(res, i, 0)),
                                  atoi(PQgetvalue(res, i, 1)));
    };
}

void Connection::setFireToStorage(CellStorage& storage)
{
    const std::string formatted_fire_query = std::format("\
    SELECT brightness, ST_X(geom), ST_Y(geom)\
    FROM FIRE f\
    WHERE ST_Intersects(f.geom, {})", analyzedPolygon());
    PGresult *res = this->runQuery(formatted_fire_query.c_str());
    printf("%d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++)
    {
        double_t k = atof(PQgetvalue(res, i, 1));
        storage.setNewState(cellState::Fire,
                                  (atof(PQgetvalue(res, i, 1)) - storage.latitudeMin) * 2,
                                  (atof(PQgetvalue(res, i, 2)) - storage.longtitudeMin) * 2);
    };
}

void Connection::setStatesToStorage(CellStorage& storage)
{
    this->setDemToStorage(storage);
    this->setBiomassToStorage(storage);
    this->setFireToStorage(storage);
};

std::pair<int, int> Connection::calculateStorageSize(){
    const std::string formatted_fire_query = std::format("\
    SELECT \
        max(x)-1, max(y)-1 \
    FROM \
		(SELECT ST_Rescale(rast, 0.0001, 0.0001) as rast from dem) as ra,\
        LATERAL ST_PixelAsPoints(ST_Clip(ra.rast, {}), 1) as q \
    WHERE  \
        ST_Intersects(ra.rast, {})", analyzedPolygon(), analyzedPolygon());
    PGresult *res = this->runQuery(formatted_fire_query.c_str());
    return std::make_pair<int, int>(atoi(PQgetvalue(res, 0, 0)), atoi(PQgetvalue(res, 0, 1)));
}

Connection::~Connection()
{
    // PQclear(result);
    // PQfinish(conn);
    PQfinish(connection_);
}