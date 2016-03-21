CC=gcc
CFLAGS=-O3 -ggdb -Wall -Werror -std=c99

.PHONY: all clean
all: swapbytes

clean:
	rm -f *.o swapbytes

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
	