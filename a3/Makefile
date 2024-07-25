TARGET = myalloc
OBJS = main.o myalloc.o list.o

CFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=199309L
CC = gcc

all: clean $(TARGET)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

valgrind:  myalloc
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./myalloc

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
