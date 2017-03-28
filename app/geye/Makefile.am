OLD_PATH	= ../../lab/stkmon

AM_CPPFLAGS = -I$(top_srcdir)/../../lab/ccutils/include
AM_CPPFLAGS += -I$(top_srcdir)/$(OLD_PATH)/include

AM_CPPFLAGS += -I$(top_srcdir)/include
AM_CPPFLAGS += $(GTK_CFLAGS)
AM_CPPFLAGS += $(XML_CFLAGS)
AM_CPPFLAGS += -D'DATADIR="$(abs_top_srcdir)/data/"'

AM_CFLAGS = -O0 -rdynamic

bin_PROGRAMS = geye_out
geye_out_SOURCES = \
		   src/monitor.c \
		   src/parser.c \
		   src/sinajs.c \
		   src/market.c \
		   src/calc.c \
		   src/event.c \
		   src/main.c

geye_out_LDADD = \
		 -lccutils -lpthread -ldl -liconv

geye_out_LDADD += \
		  $(GTK_LIBS)

geye_out_LDADD += \
		  $(XML_LIBS)

