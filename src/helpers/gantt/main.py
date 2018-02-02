import sys
import random

import matplotlib.pyplot as plt
import matplotlib.colors


# color generator
def colors():
    # h = 0
    # while True:
    #     yield matplotlib.colors.hsv_to_rgb((h/1000, 1.0, 1.0))
    #     h = (h + 100) % 1000

    # while True:
    #     yield (random.random(),
    #            random.random(),
    #            random.random())

    colors = [x['color'] for x in matplotlib.rcParams['axes.prop_cycle']]
    while True:
        for color in colors:
            yield color

# read instance
assert len(sys.argv) == 2, 'please provide path to instance file as an argument'
with open(sys.argv[1], 'r') as fh:
    file_lines = fh.read().strip().split('\n')

    operations = []
    job_id = -1
    for line in file_lines:
        parts = line.split()
        if len(parts) == 3:
            job_id = int(parts[0])
        if len(parts) == 4:
            [operation_id, _, __, duration] = parts
            operations.append((job_id, int(operation_id), int(duration)))

    operation_durations = dict([(operation_id, duration) for (_, operation_id, duration) in operations])
    operation_parents = dict([(operation_id, job_id) for (job_id, operation_id, _) in operations])

# read output of simulator
input_lines = sys.stdin.read().strip().split('\n')
assignations = [line.split() for line in input_lines]
assignations = [(operation_parents[int(operation_id)],
                 int(machine_id),
                 int(end_timestamp) - operation_durations[int(operation_id)],
                 int(end_timestamp)) for [end_timestamp,
                                          operation_id,
                                          machine_id] in assignations]

# plot
color_generator = colors()
for job_id in sorted(set(operation_parents.values())):
    job_assignations = [(machine_id, begin_timestamp, end_timestamp) for
                        (parent_id, machine_id, begin_timestamp, end_timestamp) in
                        assignations if
                        job_id == parent_id]

    machine_ids, begin_timestamps, end_timestamps = zip(*job_assignations)

    plt.hlines(machine_ids, begin_timestamps, end_timestamps, colors=next(color_generator), lw=8)

plt.margins(0.1)
plt.grid(True)
plt.show()
