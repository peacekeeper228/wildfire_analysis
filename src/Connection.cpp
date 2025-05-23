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
    // (SELECT ST_Rescale(rast, 0.00025, 0.00025) as rast from dem_s) r,
    // (SELECT ST_Rescale(rast, 0.0005, 0.0005) as rast from dem_usa) r,

    const std::string formatted_dem_query = std::format("\
    SELECT x-1, y-1, val\
    FROM(\
        SELECT\
            q.*\
        FROM\
            (SELECT ST_Rescale(rast, 0.0005, 0.0005) as rast from dem_usa) r,\
            LATERAL ST_PixelAsCentroids(ST_Clip(r.rast, {}), 1) as q\
        WHERE \
            ST_Intersects(r.rast, {})\
            ) as t\
    WHERE val > 0 and x < {} and y < {};", analyzedPolygon(), analyzedPolygon(),  getXArea(), getYArea());

    printf("Query started: %d\n", 1);
    PGresult *res = this->runQuery(formatted_dem_query.c_str());
    
    printf("dem %d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++)
    {
        // auto coordPairs = storage.getRelativePosition();
        // auto cell = storage.checkAndGetCell(atoi(PQgetvalue(res, i, 0)), atoi(PQgetvalue(res, i, 1)));
        storage.setAltitudeToCell(
            atoi(PQgetvalue(res, i, 0)),
            atoi(PQgetvalue(res, i, 1)),
            atoi(PQgetvalue(res, i, 2)));
    }
}

void Connection::setBiomassToStorage(CellStorage& storage)
{
    // (SELECT ST_Rescale(rast, 0.0005, 0.00025) as rast from biomass_s2) r,
    // (SELECT ST_Rescale(rast, 0.00025, 0.00015) as rast from biomass_usa2) r,
    const std::string formatted_biomass_query = std::format("\
    SELECT x-1, y-1, val\
    FROM(\
        SELECT\
            q.*\
        FROM\
            (SELECT ST_Rescale(rast, 0.00025, 0.00015) as rast from biomass_usa2) r,\
            LATERAL ST_PixelAsCentroids(ST_Clip(r.rast, {}), 1) as q\
        WHERE \
            ST_Intersects(r.rast, {})\
            ) as t\
    WHERE val > 80 and x < {} and y < {};", analyzedPolygon(), analyzedPolygon(), getXArea(), getYArea());
    PGresult *res = this->runQuery(formatted_biomass_query.c_str());
    printf("biomass %d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++)
    {
        // double_t k = atof(PQgetvalue(res, i, 1));
        storage.setNewState(cellState::Tree,
                                  atoi(PQgetvalue(res, i, 0)),
                                  atoi(PQgetvalue(res, i, 1)));
    };
}

void Connection::setFireToStorage(CellStorage& storage)
{
    // 70 60.5
    // -124 54
    const std::string formatted_fire_query = std::format("\
    SELECT brightness,\
        ROUND(ST_Distance(f.geom::geography, ST_SetSRID(ST_MakePoint('-124', ST_Y(f.geom)), 4326)::geography)/30)-500,\
        ROUND(ST_Distance(f.geom::geography, ST_SetSRID(ST_MakePoint(ST_X(f.geom),'54'), 4326)::geography)/30)-500\
    FROM FIRE f\
    WHERE ST_Intersects(f.geom, {})", analyzedPolygon());
    PGresult *res = this->runQuery(formatted_fire_query.c_str());
    printf("%d\t", PQntuples(res));
    for (int i = 0; i < PQntuples(res); i++)
    {
        // double_t k = atof(PQgetvalue(res, i, 1));
        storage.setNewState(cellState::Fire,
                                  (atoi(PQgetvalue(res, i, 1))),
                                  (atoi(PQgetvalue(res, i, 2))));
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