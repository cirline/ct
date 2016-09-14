CFLAGS	+= $(shell pkg-config --libs --cflags gtk+-2.0)

fout	:= a.out

objs	:= gtk_version.o

$(fout):$(objs)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o *.out

ctags:
	ctags -I __THROW --file-scope=yes --langmap=c:+.h --languages=c,c++ --links=yes --c-kinds=+p --fields=+S  -R /usr/include /usr/local/include .

