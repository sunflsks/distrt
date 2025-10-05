CFLAGS="-I/opt/homebrew/opt/libomp/include -fopenmp -Xpreprocessor -std=c++23 -ggdb3 -O2"
all:
	c++ *.cpp -o rt \
	  -L/opt/homebrew/opt/libomp/lib -lomp \
	  -Wl,-rpath,/opt/homebrew/opt/libomp/lib
