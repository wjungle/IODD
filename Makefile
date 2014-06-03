CC = gcc

CFLAGS = -g

EXEC = test
OBJS = lookdir.o test.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS)

lookdir.o: lookdir.c lookdir.h
	$(CC) -c lookdir.c $(CFLAGS)

test.o: test.c
	$(CC) -c test.c $(CFLAGS)

clean:
	rm -f test *.o
