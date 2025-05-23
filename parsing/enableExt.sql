CREATE EXTENSION postgis_raster;

CREATE TABLE IF NOT EXISTS FIRE
(
    brightness NUMERIC(3),
    geom geometry(Point, 4326)
)