from utils import BoundingBox
from datetime import date, timedelta
import requests
import psycopg2
from abc import ABC, abstractmethod

def _read_token():
    return "5685909bb99ad93463136579663de02c"

def resolve_data_type(parse_date: date):
    data_type_SP = "SP"
    data_type_NRT = "NRT"
    if parse_date < (date.today() - timedelta(days=180)):
        return data_type_SP
    else:
        return data_type_NRT   

class IFireDataSet(ABC):

    @abstractmethod
    def is_valid(confidence: str|int) -> bool:
        pass

class VIIRS(IFireDataSet):
    method = "VIIRS_SNPP"

    def is_valid(self, confidence: str) -> bool:
        return confidence == "n" or confidence == "h"

class MODIS(IFireDataSet):
    method = "MODIS"

    def is_valid(self, confidence: int) -> bool:
        return int(confidence) > 30

def parse(dataset: IFireDataSet, parse_date: date, bb: BoundingBox):

    link = f"https://firms.modaps.eosdis.nasa.gov/api/area/csv/{_read_token()}/{dataset.method}_{resolve_data_type(parse_date)}/{bb.W},{bb.S},{bb.E},{bb.N}/1/{parse_date:%Y-%m-%d}"

    sess = requests.Session()
    response = sess.get(link, allow_redirects=True)
    print(response.text)

    conn = psycopg2.connect("dbname=postgis user=postgis password=postgis port=5431")
    cursor = conn.cursor()

    for i in response.text.splitlines()[1:]:
        line = i.split(',')
        if not dataset.is_valid(line[9]):
            continue
        print(f"lat: {line[0]}, lon {line[1]}, confidence {line[9]}")
        cursor.execute("INSERT INTO fire VALUES (%s, ST_GeomFromText('POINT(%s %s)', 4326));", (int(float(line[11])), float(line[1]), float(line[0])))

    conn.commit()
    cursor.close()
    conn.close()

if __name__ == "__main__":
    parse(MODIS(), date(2024, 12, 1), BoundingBox(70, 10, 30, 40))