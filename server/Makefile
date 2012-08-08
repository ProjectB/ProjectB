CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		main.o

LIBS =

TARGET = all

MAIN = main

HTTPSERVER = httpServer

SIMPLEWSSERVER = simpleWSSever

all: $(MAIN) $(HTTPSERVER) $(SIMPLEWSSERVER)

$(MAIN):	$(OBJS)
	$(CXX) -o $(MAIN) $(OBJS) $(LIBS)

$(HTTPSERVER): PracticalSocket.o HTTPServer.o
	$(CXX) -o $(HTTPSERVER) PracticalSocket.o HTTPServer.o $(LIBS)

$(SIMPLEWSSERVER): PracticalSocket.o SimpleWebSocketServer.o
	$(CXX) -o $(SIMPLEWSSERVER) PracticalSocket.o SimpleWebSocketServer.o $(LIBS)

clean:
	rm -f $(OBJS) $(MAIN) $(HTTPSERVER) $(SIMPLEWSSERVER) *.o
