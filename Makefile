CC = gcc
CXXFLAGS = -Wall -Wextra --pedantic
LDFLAGS = -lstdc++
TARGET = cacheSim
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(SRC) -lstdc++

clean:
	rm -f $(TARGET) 

