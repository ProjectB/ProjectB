CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		main.o

LIBS =

TARGET = all

MAIN = main

HTTPSERVER = httpServer

SIMPLEWSSERVER = simpleWSServer

all: $(SIMPLEWSSERVER)
others: $(MAIN) $(HTTPSERVER) 

$(MAIN):	$(OBJS)
	$(CXX) -o $(MAIN) $(OBJS) $(LIBS)

$(HTTPSERVER): PracticalSocket.o HTTPServer.o
	$(CXX) -o $(HTTPSERVER) PracticalSocket.o HTTPServer.o $(LIBS)

$(SIMPLEWSSERVER): clientInterface.o PracticalSocket.o SimpleWebSocketServer.o SHA1.o base64.o
	$(CXX) -o $(SIMPLEWSSERVER) clientInterface.o PracticalSocket.o SimpleWebSocketServer.o SHA1.o base64.o $(LIBS) -lpthread

clean:
	rm -f $(OBJS) $(MAIN) $(HTTPSERVER) $(SIMPLEWSSERVER) *.o
