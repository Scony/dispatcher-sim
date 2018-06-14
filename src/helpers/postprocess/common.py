import os


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


def filepath_to_metadata(filepath):
    filename = os.path.basename(filepath)
    parts = filename.split('-')
    algorithm = parts[0]
    instance_name = parts[-1]
    instance_id = os.path.join(os.path.dirname(filepath), instance_name)
    metadata = {
        'algorithm': algorithm,
        'instance_name': instance_name,
        'instance_id': instance_id,
    }
    parsed_opts = [(opt[0], opt[1:]) for opt in parts[1:-1]]
    metadata.update(dict(parsed_opts))

    return metadata


def content_to_tuple(content):
    first_line = content.split('\n')[0]
    numbers_list = [float(number) for number in first_line.split()]
    return tuple(numbers_list)


def discover_results_in_directory(directory_path):
    results = []

    for root, dirs, files in os.walk(directory_path):
        for feile in files:
            path = os.path.join(root, feile)
            with open(path, 'r') as fh:
                content = fh.read()
            content_tuple = content_to_tuple(content)
            if content_tuple == ():
                continue
            metadata = {
                'path': path,
                'content': content_tuple,
            }
            metadata.update(filepath_to_metadata(path))
            results.append(metadata)

    return results
