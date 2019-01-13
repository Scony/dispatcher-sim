JOBS_NUM = $(shell expr $(shell nproc) \* 2)

all: build

ut:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug .. && make -j $(JOBS_NUM) simulator-ut
	./bin/simulator-ut

benchmarks:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug .. && make -j $(JOBS_NUM) simulator-benchmarks-run

uat:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug .. && make -j $(JOBS_NUM)
	tox -c test/py

tests: ut uat

build:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release .. && make -j $(JOBS_NUM)

hamulator:
	$(MAKE) -C hamulator

hamulator-profiling:
	$(MAKE) -C hamulator profiling

clean:
	find -name '*~' | xargs rm -f
	rm -rf build bin

.PHONY: clean ut build benchmarks uat tests hamulator
