build:
	cmake -B build
	cmake --build build -j 32 -v


run:
	cmake -B build
	cmake --build build -j 32 -v
	./build/source/tutorials/hello-triangle/hello-triangle

run-wasm:
	emcmake cmake -DUSE_GENERATED_INDEX_HTML=TRUE -B build
	cmake --build build -j 32
	python -m http.server -d ./build/source/tutorials/hello-triangle

clean:
	rm -rf build
	rm -rf install
