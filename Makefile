CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		ProjectB.o

LIBS =

TARGET =	Server

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
