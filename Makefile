AR = ar
CXX = g++
CXXFLAGS = -std=c++2a -Wall -O2 -march=x86-64 -fno-rtti
LDFLAGS = -s -static -lwinhttp

SRC = Base64.cpp HttpTransmit.cpp
OBJ = $(SRC:.cpp=.o)
DEST = HttpTransmit.a

build: fxms http_request $(OBJ)
	$(AR) r -o $(DEST) $(OBJ) Fxms.cpp/Fxms.a HttpRequest.cpp/HttpRequest.a

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

fxms:
	$(MAKE) -C Fxms.cpp

http_request:
	$(MAKE) -C HttpRequest.cpp

clean:
	$(MAKE) -C Fxms.cpp clean
	$(MAKE) -C HttpRequest.cpp clean
	rm -f $(OBJ) $(DEST)
