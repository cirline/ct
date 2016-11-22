AM_CPPFLAGS = -I$(top_srcdir)/../ccutils/include
AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += $(GTK_CFLAGS)
AM_CPPFLAGS += $(XML_CFLAGS)
AM_CPPFLAGS += -D'DATADIR="$(abs_top_srcdir)/data/"'

AM_CFLAGS = -O0

bin_PROGRAMS = stkmon_out
stkmon_out_SOURCES = src/main.c \
		     src/event.c \
		     src/calc.c \
		     src/ui.c \
		     src/parser.c \
		     src/stock_list.c \
		     src/sinajs.c

stkmon_out_LDADD = -lccutils -lpthread -ldl -liconv
stkmon_out_LDADD += $(GTK_LIBS)
stkmon_out_LDADD += $(XML_LIBS)

