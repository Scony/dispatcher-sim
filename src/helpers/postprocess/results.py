import sys
import os

from common import *

if len(sys.argv) == 1:
    print('Missing directory param')
    sys.exit(1)

directory = sys.argv[1]
files = os.listdir(directory)
results = read_algorithm_results_per_instance(directory, files)
normalize_results_per_instance(results)
finalResults = calculate_algorithm_results(results)

keys = finalResults.keys()
print(';'.join(keys))
for i in range(max([len(l) for l in finalResults.values()])):
    rowVals = [str(finalResults[key][i][0]) if len(finalResults[key]) > i else '' for key in keys]
    print(';'.join(rowVals))
