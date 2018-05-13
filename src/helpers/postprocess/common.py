def colAvg(matrix):
    assert len(matrix) > 0
    colSums = matrix[0]
    for row in matrix[1:]:
        for col in range(len(row)):
            colSums[col] += row[col]
    colAvgs = [colSum/float(len(matrix)) for colSum in colSums]

    return colAvgs


def read_algorithm_results_per_instance(directory, files):
    results = {}

    for feile in files:
        algorithm = feile.split('-')[0]
        run = '-'.join(feile.split('-')[1:])

        with open('%s/%s' % (directory, feile)) as fh:
            dataLine = fh.read().split('\n')[0].split(' ')[0]
            data = [float(x) for x in dataLine.split()]
            if len(data) > 0:
                if run not in results:
                    results[run] = {}
                results[run][algorithm] = data

    return results


def normalize_results_per_instance(results):
    for run in results:
        colMins = list(results[run].values())[0][:]
        for row in list(results[run].values())[1:]:
            for col in range(len(row)):
                colMins[col] = min(row[col], colMins[col])

        for algorithm in results[run]:
            for col in range(len(results[run][algorithm])):
                results[run][algorithm][col] = results[run][algorithm][col] / colMins[col]


def calculate_algorithm_results(results):
    finalResults = {}

    for run in results:
        for algorithm, result in results[run].items():
            if algorithm not in finalResults:
                finalResults[algorithm] = []
            finalResults[algorithm].append(result)

    return finalResults
