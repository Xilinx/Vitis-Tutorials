HEADERS=common.h filters.h types.h kernels.h constants.h
SOURCES=common.cpp convolve.cpp main.cpp grayscale_kernel.cpp convolve_kernel.cpp
OBJECTS=$(addprefix build/,$(SOURCES:.cpp=.o))

CXX_FLAGS=-std=c++0x -O3 -pg
LD_FLAGS=-O3 -pg

convolve: $(OBJECTS)
	$(CXX) $(LD_FLAGS) $^ -o $@

build/%.o: %.cpp $(HEADERS)
	@mkdir -p build
	$(CXX) $(CXX_FLAGS) $< -c -o $@

golden: convolve
	./convolve ../video.mp4 -o output_full.mp4; 
	./convolve ../video.mp4 --scale 512 10 --nframes 1 -o output_small.mp4; 
	./convolve ../video.mp4 --scale 512 40 --nframes 1 -o output_small_40.mp4;
	cp output_full.mp4 ../golden_out_full.mp4;
	cp output_small.mp4 ../golden_out_small.mp4;
	cp output_small_40.mp4 ../golden_out_small_40.mp4;

clean:
	rm -f build/*.o
