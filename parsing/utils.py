from abc import ABC, abstractmethod
from datetime import date, timedelta
from enum import Enum, auto

class IBoundingBox(ABC):
    N : float
    S : float
    E : float
    W : float

    @abstractmethod
    def intersects(self, other: 'BoundingBox'):
        pass

    @abstractmethod
    def covers(self, other: 'BoundingBox'):
        pass

class BoundingBox:
    def __init__(self, N: float, S: float, W: float, E: float) -> None:
        self.N = N
        self.S = S
        self.E = E
        self.W = W

    def inersects(self, other: 'BoundingBox'):
        return not (self.N > other.S or self.S < self.N or self.E < other.W or self.W > other.E)
    
    def covers(self, other: 'BoundingBox'):
        return not (self.N > other.S and self.S < self.N and self.E < other.W and self.W > other.E)
        
class DataType(Enum):
    DEM = auto()
    BIOMASS = auto()
    WEATHER = auto()
    FIRE = auto()

class IDataset(ABC):
    identifier : str
    geo_coverage : IBoundingBox
    data_type: DataType
    name: str
    link: str

    @abstractmethod
    def process_file_names(self, files: list[str]) -> list[str]:
        pass

class ASTER_GDEM(IDataset):
    identifier = "short_name=ASTGTM&version=003"
    geo_coverage = BoundingBox(82.0, -83, -180, 180)
    data_type = DataType.DEM
    name = "aster"
    link = "https://data.nasa.gov/dataset/ASTER-Global-Digital-Elevation-Model-V003/76ry-dw3q/about_data"
    
    def process_file_names(self, files):
        return [i for i in files if i[-4:] == ".tif"]

class ForestsBiomass(IDataset):
    identifier = "echo_collection_id=C2756302505-ORNL_CLOUD"
    geo_coverage = BoundingBox(78.53, 43.71, -179.82, 178.4)
    data_type = DataType.BIOMASS
    name = "forest"
    link = "https://data.nasa.gov/dataset/Aboveground-Biomass-Density-for-High-Latitude-Fore/95et-uf7n/about_data"

    def process_file_names(self, files):
        return [i for i in files if i[-4:] == ".tif"]

class WeatherAIRS(IDataset):
    identifier = f"echo_collection_id=C2041968414-GES_DISC&created_at=2024-11-02T00:00:00.000Z"
    geo_coverage = BoundingBox(90.0, -90, -180, 180)
    data_type = DataType.WEATHER
    name = "weather"
    link = "https://data.nasa.gov/dataset/TROPESS-AIRS-Aqua-L2-Atmospheric-Temperature-for-F/bazj-hnkd/about_data"

    def process_file_names(self, files):
        return [i for i in files if i[-3:] == ".nc" and (date.today() - timedelta(days=16)).strftime('%Y%m%d') in i]

# class VIIRS(IDataset):
#     identifier = "echo_collection_id=C3273639213-GES_DISC"
#     geo_coverage = BoundingBox(80.0, -80, -180, 180)
#     data_type = DataType.FIRE
#     name = "viirs"
#     link = "https://data.nasa.gov/dataset/VIIRS-NOAA-21-I-Band-375-m-Active-Fire-Product-NRT/cm6c-4i3b/about_data"

#     def process_file_names(self, files):
#         return [files[0]]

collections :list[IDataset] = [
    ASTER_GDEM(),
    ForestsBiomass(),
    WeatherAIRS()
    # VIIRS()
] 
