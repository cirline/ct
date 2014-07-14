CC := gcc
LIBS += -lpthread -ldl
CFLAGS += -Wall

SOFT := mysh
OBJS := $(patsubst %.c, %.o, $(wildcard *.c))

$(SOFT): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o $(SOFT)
