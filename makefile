all: build

test:
	mkdir -p build && cd build && cmake -DBUILD_TESTING=ON .. && make -j
	./build/test/simulator-test

build:
	mkdir -p build && cd build && cmake -DBUILD_TESTING=OFF .. && make -j

clean:
	find -name '*~' | xargs rm -f
	rm -rf build

.PHONY: clean test build
