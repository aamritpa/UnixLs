CC = gcc
CFLAGS =
PROG = UnixLs
OBJS = UnixLs.o 

run: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

UnixLs.o: UnixLs.h

clean:
	rm *.o UnixLs
