# psx emulator makefile

BIN  = main
OBJS = build/main.o build/Cpu.o build/Bios.o build/Interconnect.o \
       build/Range.o build/Quit.o

CC     = g++
CFLAGS = -g -Wall -std=c++11 

build/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

clean:
	rm -rf $(OBJS) $(BIN)

