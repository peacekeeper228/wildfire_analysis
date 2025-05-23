from CMRParser import CMSParser
from FileDownloader import FileDownloader
from utils import BoundingBox, collections, DataType, IDataset
import os
from fires import parse, VIIRS
from datetime import date

def main():
    box = BoundingBox(56, 55, 37, 38)
    parse_date = date(2024, 1, 1)

    for data_type in DataType:
        collections_to_parse : list[IDataset] = list(filter(lambda el: el.data_type == data_type, collections))
        really_parse : list[IDataset] = []
        for collection in collections_to_parse:
            if collection.geo_coverage.covers(box):
                really_parse = [collection]
                break
            elif collection.geo_coverage.intersects(box):
                really_parse.append(collection)
        
        print(f'collections to parse {data_type} are: {really_parse}')
        for collection in really_parse:
            parser = CMSParser(box, collection)

            links = parser.parse_file_names()
            print(links)
            folder_name = f"./data/{collection.data_type.value}/"
            if os.path.isdir(folder_name):
                os.rmdir(folder_name)
            os.mkdir(folder_name)
            downloader = FileDownloader(folder_name)
            downloader.download(links)

    parse(VIIRS, parse_date, box)


if __name__ == "__main__":
    main()
# box = BoundingBox(50, 49, 37, 38)

# parser = CMSParser(box, VIIRS())

# links = parser.parse_file_names()
# print(links)
