import requests

def read_token():
    return "eyJ0eXAiOiJKV1QiLCJvcmlnaW4iOiJFYXJ0aGRhdGEgTG9naW4iLCJzaWciOiJlZGxqd3RwdWJrZXlfb3BzIiwiYWxnIjoiUlMyNTYifQ.eyJ0eXBlIjoiVXNlciIsInVpZCI6ImVhcnRoZGF0YWZvcnN0dWR5IiwiZXhwIjoxNzM0OTgzMDQ0LCJpYXQiOjE3Mjk3OTkwNDQsImlzcyI6Imh0dHBzOi8vdXJzLmVhcnRoZGF0YS5uYXNhLmdvdiJ9.jrRFN_ViJAFd2aKs2lgAjcOSwo4Yf7sfUCap5eDFHz700VupWPlCJyOyFUH6VjvjB8Ystoo3Gx3ReKgn8JIspB4bdxVocPzoOKqr1xV_cHYmloK5U9wyqTWV9yQlSieAKPgbXpaGZqMP5XqZMpgVGHEZKsObUWqEJCKf8ZFFAn66uNMKA3EPsyUOGF5inVhzbWt4yC79pfIRAoNaBefqcjYFjuMAvgkRg7YmCV4l19fihYl9ubNAvSJAGrsQxRrK5RSOs2UDO5P2nksuiuiDat1Soi95h7iuGAiWZBeqeFq2_KROXf78VmNVxbyZCHQ0RfSuqmNgkjNoEtaYkLFXbA"

class FileDownloader:
    def __init__(self, folder_to_save: str = "./data/") -> None:
        self.folder_to_save = folder_to_save
        self.__token = read_token()

    def download(self, files: list[str]):
        sess = requests.Session()
        sess.headers = {'Authorization': 'Bearer {0}'.format(self.__token)}
        for i in files:
            response = sess.get(i, allow_redirects=True)
            print(response.status_code)
            if response.status_code == 200:
                filename = i.split('/')[-1]
                with open(f'{self.folder_to_save}{filename}', 'wb') as f:
                    f.write(response.content)
                


    