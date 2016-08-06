AM_CPPFLAGS = -I$(top_srcdir)/../ccutils/include
AM_CPPFLAGS += -I$(top_srcdir)/include

AM_CFLAGS = -O0

bin_PROGRAMS = stock_out
stock_out_SOURCES = src/main.c \
		    src/db_if.c \
		    src/ui.c

stock_out_LDADD = -lccutils -lpthread -ldl

