import sys

from common import discover_results_in_directory


if len(sys.argv) == 1:
    print('Missing directory param')
    sys.exit(1)

directory_path = sys.argv[1]
results_summary = discover_results_in_directory(directory_path)
print(';'.join([
    'instance_id',
    'algorithm',
    'machines',
    'iterations',
    'avg_flow',
    'med_flow',
    'min_flow',
    'max_flow',
    'stddev_flow',
]))
for result in results_summary:
    if 'i' not in result:
        result['i'] = ''
    print(';'.join([
        result['instance_id'],
        result['algorithm'],
        result['m'],
        result['i'],
        ';'.join([str(x) for x in result['content']]),
    ]))
