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

## Running
To run simple FIFO dispatcher (clairvoyant) over 20 machines type in following command:
```
cat instances/692_10.txt | ./bin/simulator fifo -m 20 -e no | ./bin/stat-desc
```

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

### Supported testcase duration estimation methods `-e`

| Value | Method |
|---|---|
| no | no estimation (clairvoyant) |
| lclv | lazy clairvoyant (clairvoyant once known) |

## Instance format
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
