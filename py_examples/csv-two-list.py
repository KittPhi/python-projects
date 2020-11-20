import csv

dates = ['2020-01-01', '2020-01-02', '2020-01-03']
urls = ['www.abc.com', 'www.cnn.com', 'www.nbc.com']

csv_file_patch = '/path/to/filename.csv'

with open(csv_file_patch, 'w') as fout:
    csv_file = csv.writer(fout, delimiter=';', lineterminator='\n')
    result_array = zip(dates, urls)
    csv_file.writerows(result_array)