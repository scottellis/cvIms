
CC = g++
CFLAGS = -Wall -Wno-unused-function

CXXFLAGS += `pkg-config opencv --cflags`

LDFLAGS += `pkg-config opencv --libs`


TARGET = cvIms


$(TARGET): main.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) main.cpp -o $(TARGET)
	
	
clean:
	rm $(TARGET)

