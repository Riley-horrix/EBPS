ARGS?=

all: generate-lib

# Remove build / doc artifacts
clean:
	@ rm -rf ./build
	@ rm -rf ./html

# Build and run test executable
test: generate-lib 
	@ cd build && \
	make test && \
	./test $(ARGS)

# Use cpplint
test-lint:
	cpplint --recursive --exclude=./build --filter=-whitespace/ending_newline,-build/namespaces,-whitespace/indent .

# Build documentation
doc:
	mkdir -p doc && \
	doxygen && \
	open ./doc/html/index.html

# Generate library
generate-lib:
	@ mkdir -p build && \
	cd build && \
	cmake .. && \
	cmake --build .

.PHONY: all test test-lint doc generate-lib clean