CXX = /opt/homebrew/opt/llvm/bin/clang++
CFLAGS=-fopenmp -Xpreprocessor -std=c++26 -ggdb3 -O2
all:
	$(CXX) *.cpp $(CFLAGS) -o rt

clean:
	rm -rf rt *.dSYM *.ppm