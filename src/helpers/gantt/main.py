import sys

import matplotlib.pyplot as plt


# read instance
assert len(sys.argv) == 2, 'please provide path to instance file as an argument'
with open(sys.argv[1], 'r') as fh:
    file_lines = fh.read().strip().split('\n')
    operations = [line.split() for line in file_lines if len(line.split()) == 4]
    operations = [[int(field) for field in operation] for operation in operations]
    operations = [(id, duration) for [id, _, __, duration] in operations]
    operations = dict(operations)


# read output of simulator
input_lines = sys.stdin.read().strip().split('\n')
assignations = [line.split() for line in input_lines]
assignations = [(int(machine_id),
                 int(finish_timestamp) - operations[int(operationId)],
                 float(finish_timestamp) - 0.3) for [finish_timestamp,
                                                     operationId,
                                                     machine_id] in assignations]
machine_ids, begin_timestamps, finish_timestamps = zip(*assignations)

# plot (# https://stackoverflow.com/questions/21323032/python-plot-gantt-chart)
plt.hlines(machine_ids, begin_timestamps, finish_timestamps, colors="red", lw=8)
plt.margins(0.1)
plt.grid(True)
plt.show()
