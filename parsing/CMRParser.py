from utils import IBoundingBox, IDataset
import requests

class CMSParser:
    def __init__(self, box: IBoundingBox, dataset: IDataset) -> None:
        self.box = box
        self.dataset = dataset

    def parse_file_names(self) -> list[str]:
        req = requests.get(f'https://cmr.earthdata.nasa.gov/search/granules.json?{self.dataset.identifier}&page_size=2000&pageNum=1&bounding_box={self.box.W},{self.box.S},{self.box.E},{self.box.N}').json()['feed']['entry'] 
        files = [g['links'][0]['href'] for g in req]
        return self.dataset.process_file_names(files)