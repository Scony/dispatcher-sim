all: build

ut:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=ON .. && make -j simulator-ut
	./bin/simulator-ut

benchmarks:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=ON .. && make -j simulator-benchmarks-run

uat: build
	tox -c test/py

tests: ut uat

build:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=OFF .. && make -j

clean:
	find -name '*~' | xargs rm -f
	find bin/ -executable -type f | xargs rm -f
	rm -rf build

.PHONY: clean ut build benchmarks uat tests
