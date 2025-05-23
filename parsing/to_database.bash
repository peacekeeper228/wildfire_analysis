raster2pgsql -F -I -C -t 10000x10000  /home/database/1/*.tif public.dem > /home/database/out.sql
psql -d postgis -f /home/database/out.sql
raster2pgsql -F -I -C -t 10000x10000  /home/database/2/*.tif public.biomass > /home/database/out.sql
psql -d postgis -f /home/database/out.sql
