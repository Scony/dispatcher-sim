import sys
import os

def colAvg(matrix):
    colSums = matrix[0]
    for row in matrix[1:]:
        for col in range(len(row)):
            colSums[col] += row[col]
    colAvgs = [colSum/float(len(matrix)) for colSum in colSums]

    return colAvgs

if len(sys.argv) == 1:
    print('Missing directory param')
    sys.exit(1)

directory = sys.argv[1]
files = os.listdir(directory)
instances = set([feile.split('-')[-1] for feile in files])
results = {}

# read data
for feile in files:
    algorithm = feile.split('-')[0]
    run = '-'.join(feile.split('-')[1:])

    if run not in results:
        results[run] = {}

    with open('%s/%s' % (directory, feile)) as fh:
        dataLine = fh.read().split('\n')[0].split(' ')[0]
        data = [float(x) for x in dataLine.split()]
        if len(data) > 0:
            results[run][algorithm] = data

# normalize results per instance
for run in results:
    colMins = list(results[run].values())[0][:]
    for row in list(results[run].values())[1:]:
        for col in range(len(row)):
            colMins[col] = min(row[col], colMins[col])

    for algorithm in results[run]:
        for col in range(len(results[run][algorithm])):
            results[run][algorithm][col] = results[run][algorithm][col] / colMins[col]

# calculate final results
finalResults = {}

for run in results:
    for algorithm, result in results[run].items():
        if algorithm not in finalResults:
            finalResults[algorithm] = []
        finalResults[algorithm].append(result)

keys = finalResults.keys()
print(';'.join(keys))
for i in range(max([len(l) for l in finalResults.values()])):
    rowVals = [str(finalResults[key][i][0]) if len(finalResults[key]) > i else '' for key in keys]
    print(';'.join(rowVals))
