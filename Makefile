
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

# run-wasm:
# 	emcmake cmake -B build
# 	cmake --build build -j 32
# 	python -m http.server -d ./build

clean:
	rm -rf build
	rm -rf install
