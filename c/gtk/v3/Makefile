CFLAGS	+= $(shell pkg-config --libs --cflags gtk+-3.0)

# f not include the extension .c
f	?= gtk_version

fout	:= a.out

objs	:= $(f).o

$(fout):$(objs)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o *.out

ctags:
	ctags -I __THROW --file-scope=yes --langmap=c:+.h --languages=c,c++ --links=yes --c-kinds=+p --fields=+S  -R /usr/include /usr/local/include .

