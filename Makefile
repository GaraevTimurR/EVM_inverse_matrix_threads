all: hello
GFLAGS=-fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wno-suggest-attribute=format -lm -ldl -lpthread -O3

hello: main.o ErrorNorm.o Gauss1.o MatIn.o MatOut.o
	g++ $(GFLAGS) main.o ErrorNorm.o Gauss1.o MatIn.o MatOut.o -lm

main.o: main.cpp
	g++ $(GFLAGS) -c main.cpp

Gauss1.o: Gauss1.cpp
	g++ $(GFLAGS) -c Gauss1.cpp

ErrorNorm.o: ErrorNorm.cpp
	g++ $(GFLAGS) -c ErrorNorm.cpp

MatOut.o: MatOut.cpp
	g++ $(GFLAGS) -c MatOut.cpp

MatIn.o: MatIn.cpp
	g++ $(GFLAGS) -c MatIn.cpp

clean:
	rm -rf *.o hello                          
