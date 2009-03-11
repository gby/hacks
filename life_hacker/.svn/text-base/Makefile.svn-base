
CFLAGS := -g -pthread -Wall
LDFLAGS := -g -pthread -Wall

.PHONY: all clean

all: server client

server: server.o life.o

client: client.o

clean:
	@rm -f *.o server client
