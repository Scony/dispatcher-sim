import sys
import os
import csv

from common import *


if len(sys.argv) == 1:
    print('Missing directory param')
    sys.exit(1)

directory = sys.argv[1]
files = os.listdir(directory)
algorithm_jfins = {}
for feile in files:
    path = os.path.join(directory, feile)
    algorithm = feile.split('-')[0]
    with open(path, 'r') as fh:
        if algorithm not in algorithm_jfins:
            algorithm_jfins[algorithm] = []
        jfins = fh.read().strip().split('\n')
        algorithm_jfins[algorithm] += jfins

csv_writer = csv.writer(sys.stdout, delimiter=';')
csv_writer.writerow(algorithm_jfins.keys())
csv_writer.writerows(zip(*algorithm_jfins.values()))
