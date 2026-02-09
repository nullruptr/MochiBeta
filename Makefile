compile:
	rm -rf build
	mkdir build
	cd build && \
	cmake .. \
	  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
	  -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake \
	  -DCMAKE_BUILD_TYPE=Release && \
	make
