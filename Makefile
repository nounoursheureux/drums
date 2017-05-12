CC = g++
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lportaudio
OBJ = main.o sound.o sample.o

all: drums

drums: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)
