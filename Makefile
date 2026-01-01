LIBS =
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean install

default: logger client
all: default

HEADERS = protocol.h
LOGGER_OBJS = server.o 
CLIENT_OBJS = client.o

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

logger: $(LOGGER_OBJS)
	$(CC) $(LOGGER_OBJS) -Wall $(LIBS) -o $@

client: $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f logger client
