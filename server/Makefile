CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS = main.o Server.o GameServer.o ClientConnection.o

LIBS = lib/sha1.o lib/PracticalSocket.o lib/base64.o

TARGET = main

all: $(TARGET)

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	rm -f $(TARGET) *.o
