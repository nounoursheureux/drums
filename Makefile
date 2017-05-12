CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lportaudio
OBJ = main.o

all: drums

drums: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)
