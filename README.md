# dispatcher-sim
Testing workload dispatcher simulator.

## 1. Buidilng
```
make
```

## 2. Testing
```
make test
```
Note that you will need GTest & GMock

## 3. Running
To run simple FIFO dispatcher (clairvoyant) over 20 machines type in following command:
```
cat instances/692_10.txt | ./build/source/simulator fifo -m 20 -e no | ./build/helpers/stat-desc/stat-desc
```

## 4. Instance format
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
