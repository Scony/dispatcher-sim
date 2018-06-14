import sys

from common import discover_results_in_directory


if len(sys.argv) == 1:
    print('Missing directory param')
    sys.exit(1)

directory_path = sys.argv[1]
results_summary = discover_results_in_directory(directory_path)
summary = {}

for result in results_summary:
    key = '%s_m%s' % (result['instance_id'], result['m'])
    if key not in summary:
        summary[key] = []
    summary[key].append(result)

for results in summary.values():
    sjlo = [result for result in results if result['algorithm'] == 'sjlo'][0]
    reference_cost = sjlo['content'][0]
    results.remove(sjlo)
    for result in results:
        print('%s;%s' % (result['i'], str(result['content'][0]/reference_cost)))
