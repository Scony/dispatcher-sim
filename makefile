all: build

test:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=ON .. && make -j simulator-ut
	./bin/simulator-ut

build:
	mkdir -p build && cd build && cmake -DBUILD_TESTS=OFF .. && make -j

clean:
	find -name '*~' | xargs rm -f
	rm -rf build bin

.PHONY: clean test build
