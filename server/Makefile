CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		main.o

LIBS =

TARGET =	all

MAIN = main

HTTPSERVER = httpServer

all: $(MAIN) $(HTTPSERVER)

$(MAIN):	$(OBJS)
	$(CXX) -o $(MAIN) $(OBJS) $(LIBS)

$(HTTPSERVER): PracticalSocket.o HTTPServer.o
	$(CXX) -o $(HTTPSERVER) PracticalSocket.o HTTPServer.o $(LIBS)

clean:
	rm -f $(OBJS) $(MAIN) $(HTTPSERVER) *.o
