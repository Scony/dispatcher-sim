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

for algorithm, matrix in finalResults.items():
    finalResults[algorithm] = colAvg(finalResults[algorithm])

crucialResults = [(v[0], k) for k, v in finalResults.items()]
for tuple in sorted(crucialResults):
    print(tuple)
