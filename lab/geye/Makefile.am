AM_CPPFLAGS = -I$(top_srcdir)/../ccutils/include
AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += $(GTK_CFLAGS)
AM_CPPFLAGS += $(XML_CFLAGS)
AM_CPPFLAGS += -D'DATADIR="$(abs_top_srcdir)../stkmon/data/"'

AM_CFLAGS = -O0 -rdynamic

bin_PROGRAMS = geye_out
geye_out_SOURCES = \
		     ../stkmon/src/main.c \
		     ../stkmon/src/monitor.c \
		     ../stkmon/src/event.c \
		     ../stkmon/src/calc.c \
		     ../stkmon/src/parser.c \
		     ../stkmon/src/stock_list.c \
		     ../stkmon/src/sinajs.c

geye_out_LDADD = \
		 -lccutils -lpthread -ldl -liconv

geye_out_LDADD += \
		  $(GTK_LIBS)

geye_out_LDADD += \
		  $(XML_LIBS)

