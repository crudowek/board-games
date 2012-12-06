CXX=g++
CXXFLAGS=-D_WIN32_WINNT=0x0501

all: exception.o

exception.o: exception.cpp
	${CXX} -c ${CXXFLAGS} exception.cpp -o exception.o
	
clean:
	rm -f *.o