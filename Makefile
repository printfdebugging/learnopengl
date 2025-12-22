build:
	cmake -B build
	cmake --build build -j 32 -v


run:
	cmake -B build
	cmake --build build -j 32 -v
	./build/source/game/game

# run-wasm:
# 	cd /home/printfdebugging/repos/website && ./scripts/generate-graphics.sh
# 	emcmake cmake -B build
# 	cmake --build build -j 32
# 	python -m http.server -d ./build/source/tutorials/textures

clean:
	rm -rf build
	rm -rf install
