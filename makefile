all: build

ut:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=ON .. && make -j simulator-ut
	./bin/simulator-ut

benchmarks:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=ON .. && make -j simulator-benchmarks-run

build:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=OFF .. && make -j

clean:
	find -name '*~' | xargs rm -f
	rm -rf build bin

.PHONY: clean ut build benchmarks
