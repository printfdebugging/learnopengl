
.PHONY: build
build:
	cmake -B build
	cmake --build build -j 32 -v


run:
	cmake -B build
	cmake --build build -j 32 -v
	./build/executable

install: build
	cmake --install build --prefix install

clean:
	rm -rf build
	rm -rf install
