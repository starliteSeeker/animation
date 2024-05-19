GCC = gcc -Wall
LDFLAGS = -lncurses
DEBUG = gcc -g -Wall
EXE = animation.out

SRCS = main.c bounce.c snow.c combination_lock.c ripple.c
OBJS = $(SRCS:%.c=%.o)

.c.o: 
	$(GCC) -c $*.c 

all: $(OBJS)
	$(GCC) $(OBJS) -o $(EXE) $(LDFLAGS)

debug:
	$(DEBUG) $(SRCS) -o $(EXE) $(LDFLAGS)

clean:
	rm -f $(EXE) *.o
