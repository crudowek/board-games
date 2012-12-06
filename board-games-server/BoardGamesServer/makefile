#board games makefile
CXX:=g++
UNAME=$(shell uname -s)

ifeq (${UNAME},WindowsNT)
BOOST='c:\mingw_boost_1_47_0'
CRYPTO='c:\CryptoPP'
VP=c:\vp
CXXFLAGS:=-I ${VP} -I ${CRYPTO} -I ${BOOST} -D_WIN32_WINNT=0x0501 -g -DDEBUG
PROGRAM=program.exe
VPLIB=${VP}'\vp'
BOOSTLIB=${BOOST}'\stage\lib'
CRYPTOLIB=${CRYPTO}'\cryptopp'
#LINK=-lvp -llibboost_filesystem-mgw46-1_47 -llibboost_system-mgw46-1_47 -lws2_32 -lcryptopp -llibboost_random-mgw46-1_47 -llibboost_iostreams-mgw46-1_47 -lmswsock -llibboost_thread-mgw46-mt-1_47
LINK= -lws2_32 -lmswsock -Wl,-static -lvp -lboost_filesystem-mgw46-1_47 -lboost_system-mgw46-1_47  -lcryptopp -lboost_random-mgw46-1_47 -lboost_iostreams-mgw46-1_47 -lboost_thread-mgw46-mt-1_47
else
BOOST='../../boost_1_47_0'
CRYPTO='../../cryptopp561'
VP='../vp'
BOOSTLIB=${BOOST}'/stage/lib'
CRYPTOLIB=${CRYPTO}'/cryptopp'
VPLIB=${VP}'/vp'
CXXFLAGS:=-I ${VP} -I ${CRYPTO} -I ${BOOST} -g -DDEBUG
PROGRAM=program

#LINK=-ldl -Wl,-Bstatic -lvp -lboost_filesystem -lboost_system -lcryptopp -lboost_random -lboost_iostreams -lboost_thread
LINK=-ldl -lvp -lboost_filesystem -lboost_system -lcryptopp -lboost_random -lboost_iostreams -lboost_thread -lpthread

endif


OBJECTS:=main.o roomKeeper.o userDb.o server.o keeper.o client.o matchKeeper.o constants.o moduleLoader.o parameter.o security.o exception.o matchClient.o match.o

${PROGRAM}: ${OBJECTS}
	${CXX}  ${OBJECTS} -L${BOOSTLIB} -L${CRYPTOLIB} -L${VPLIB} ${LINK} -o ${PROGRAM}

modlib: libmod.a

libmod.a: exception.o module.o gameinfo.o constants.o
	ar rcs libmod.a exception.o module.o gameinfo.o constants.o

main.o: main.cpp
	${CXX} -c ${CXXFLAGS} main.cpp
  
roomKeeper.o: roomKeeper.cpp
	${CXX} -c ${CXXFLAGS} roomKeeper.cpp
	
userDb.o: userDb.cpp
	${CXX} -c ${CXXFLAGS} userDb.cpp
	
server.o: server.cpp
	${CXX} -c ${CXXFLAGS} server.cpp
	
keeper.o: keeper.cpp
	${CXX} -c ${CXXFLAGS} keeper.cpp

client.o: client.cpp
	${CXX} -c ${CXXFLAGS} client.cpp

matchKeeper.o: matchKeeper.cpp
	${CXX} -c ${CXXFLAGS} matchKeeper.cpp

constants.o: constants.cpp
	${CXX} -c ${CXXFLAGS} constants.cpp

moduleLoader.o: moduleLoader.cpp
	${CXX} -c ${CXXFLAGS} moduleLoader.cpp
	
parameter.o: parameter.cpp
	${CXX} -c ${CXXFLAGS} parameter.cpp

exception.o: exception.cpp
	${CXX} -c ${CXXFLAGS} exception.cpp

security.o: security.cpp
	${CXX} -c ${CXXFLAGS} security.cpp

matchClient.o: matchClient.cpp
	${CXX} -c ${CXXFLAGS} matchClient.cpp
	
match.o: match.cpp
	${CXX} -c ${CXXFLAGS} match.cpp	
	
clean:
	rm -f *.o ${PROGRAM}


module.o: module.cpp
	${CXX} -c ${CXXFLAGS} module.cpp
	
gameinfo.o: gameinfo.cpp
	${CXX} -c ${CXXFLAGS} gameinfo.cpp



