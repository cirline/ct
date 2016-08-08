CC := gcc
LIBS += -lpthread -ldl
CFLAGS += -Wall

SOFT := sqli
#OBJS := $(patsubst %.c, %.o, $(wildcard *.c))
OBJS := main.o
OBJS += sqlite3.o

$(SOFT): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o $(SOFT) *.exe
