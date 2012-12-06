CXX=g++
CXXFLAGS=-g -DDEBUG -D_WIN32_WINNT=0x0501

all: escaper.o

escaper.o: escaper.cpp
	${CXX} -c ${CXXFLAGS} escaper.cpp -o escaper.o
	
clean:
	rm -f *.o
