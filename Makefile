CC = g++
CFLAGS = -Wall -Wextra -g -std=c++11
LDFLAGS = -lportaudio -lasound
OBJ = main.o sound.o sample.o midi.o sequencer.o engine.o serial.o arduino.o

all: drums

drums: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)
