# dispatcher-sim
Testing workload dispatcher simulator.

## Buidilng
```
make
```

## Testing
```
# UT
make ut

# Benchmarks
make benchmarks

# UAT (requires tox [sudo pip install tox])
make uat

# UT + UAT
make tests
```
Note that benchmarking results may be affected by CPU scaling. To control that you can use following commands:
```
sudo cpupower frequency-set --governor performance # disable
sudo cpupower frequency-set --governor powersave   # enable
```

## Running
To run simple FIFO dispatcher (clairvoyant) over 20 machines type in following command:
```
cat instances_v1/692_10.txt | ./bin/simulator fifo -m 20 -e no | ./bin/stat-desc
```

To plot Gantt chart type in following command:
```
cat instances_v1/692_10.txt | ./bin/simulator fifo -m 20 -e no -o opfins | python ./bin/gantt.py instances_v1/692_10.txt
```
note that you will need matplotlib (`sudo pip install matplotlib`) and `tk` linux package

## Run options

### Supported algorithms `algorithm`

| Value | Algorithm |
|---|---|
| random | random (`srand(time(0))`) |
| max | RR Job, Max Operation |
| min | RR Job, Min Operation |
| fifo | FIFO Job, As-is Operation |
| sjlo | Shortest Job, Longest Operation |
| sjso | Shortest Job, Shortest Operation |
| ljso | Longest Job, Shortest Operation |
| lo | Longest Operation (conflicts - Earliest Operation) |
| elo | Earliest Operation (conflicts - Longest Operation) |
| so | Shortest Operation (conflicts - Earliest Operation) |
| qopt | Optimal from Queue POV |
| qworst | Worst from Queue POV |
| jopt | Optimal Job permutation, Operations by `-l` |
| jsa | SA over Job permutation, Operations by `-l` |
| sasa | SA over Job permutation, SA over Job Operations permutation |
| sjsa | Shortest Job, SA over Job Operations permutation |
| sa | SA over all Operations |
| rvr | Random Variable Rule |
| dvr | Determininstic Variable Rule |

### Supported testcase duration estimation methods `-e`

| Value | Method |
|---|---|
| no | no estimation (clairvoyant) |
| lclv | lazy clairvoyant (clairvoyant once known) |

### Supported operation-level algorithms `-l`

| Value | Algorithm |
|---|---|
| random | Random |
| max | Longest Operation |
| min | Shortest Operation |

### Supported stdout formats `-o`

| Value | Format |
|---|---|
| jflows | Flows of consecutive Jobs |
| jstretches | Stretches of consecutive Jobs |
| opfins | Triples `[finish timestamp] [operation id] [machine id]` |

## Instance format

### V1

```
[number of test suites]

[test suite uuid] [test suite priority (1>0)] [arrival time (s)]
[number of test cases]
[test case uuid] [test case type]  [test case outcome (0 - pass)] [test case duration (s)]
[test case uuid] [test case type]  [test case outcome (0 - pass)] [test case duration (s)]
(...)

[test suite uuid] [test suite priority (1>0)] [arrival time (s)]
[number of test cases]
[test case uuid] [test case type]  [test case outcome (0 - pass)] [test case duration (s)]
[test case uuid] [test case type]  [test case outcome (0 - pass)] [test case duration (s)]
(...)

(...)
```

### V2

```
0 # for backward compatibility
2 # version of instance format
[number of test suites]
[test suite uuid] [test suite priority (1>0)] [arrival time (s)]
[test suite uuid] [test suite priority (1>0)] [arrival time (s)]
[test suite uuid] [test suite priority (1>0)] [arrival time (s)]
(...)
[number of test cases]
[test suite uuid] [test case uuid] [test case type]  [test case outcome (0 - pass)] [test case duration (s)] [required machine capacity]
[test suite uuid] [test case uuid] [test case type]  [test case outcome (0 - pass)] [test case duration (s)] [required machine capacity]
[test suite uuid] [test case uuid] [test case type]  [test case outcome (0 - pass)] [test case duration (s)] [required machine capacity]
(...)
```

## Machinefile format

### V1

```
1 # version of machinefile format
[number of machines]
[machine uuid] [capacity of machine]
[machine uuid] [capacity of machine]
[machine uuid] [capacity of machine]
(...)
```

## Examples
```
time cat instances_v1/5596_80.txt | ./bin/simulator sa -m 10 -e no -l max -i 1000 2>/dev/null | ./bin/stat-desc
time cat instances_v2/10_1.txt machines_v1/2.txt | ./bin/simulator sa -e no -i 10 -v2 | ./bin/stat-desc
```
