CXX:=g++
UNAME=$(shell uname -s)

BG=../../..

ifeq (${UNAME},WindowsNT)
BOOST=c:\mingw_boost_1_47_0
CRYPTO=c:\CryptoPP
VP=c:\vp
CXXFLAGS:=-I ${BG} -I ${VP} -I ${CRYPTO} -I ${BOOST} -D_WIN32_WINNT=0x0501
VPLIB=${VP}\vp
BOOSTLIB=${BOOST}\stage\lib
CRYPTOLIB=${CRYPTO}\cryptopp
SHARED=dummyModule.dll
LINK=-L${BG} -L${BOOSTLIB} -L${VPLIB} -lvp -lboost_system-mgw46-1_47 -lmod -lws2_32 -lmswsock -lboost_filesystem-mgw46-1_47 -lboost_system-mgw46-1_47 -lboost_random-mgw46-1_47 -lboost_iostreams-mgw46-1_47 -lboost_thread-mgw46-mt-1_47 -Wl,--out-implib,libDummyModule.a
else
BOOST=../../boost_1_47_0
CRYPTO=../../cryptopp561
VP=../vp
BOOSTLIB=${BOOST}/stage/lib
CRYPTOLIB=${CRYPTO}/cryptopp
VPLIB=${VP}/vp
CXXFLAGS:=-I ${BG} -I ${VP} -I ${CRYPTO} -I ${BOOST} -g -DDEBUG
SHARED=dummyModule.so
LINK=-L${BG} -L${BOOSTLIB} -lmod -lboost_system -lvp -lboost_filesystem -lboost_random -lboost_iostreams -lboost_thread -lpthread -Wl,--out-implib,libDummyModule.a
endif

${SHARED}: dummyModule.o module.o gameinfo.o
	${CXX} -shared  -o ${SHARED} dummyModule.o module.o gameinfo.o ${LINK}
dummyModule.o: dummyModule.cpp
	${CXX} -c ${CXXFLAGS} dummyModule.cpp

clean:
	rm -f *.o *.a ${SHARED}
