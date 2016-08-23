lib_LTLIBRARIES = libccutils.la
libccutils_la_SOURCES = src/log.c \
			src/db.c \
			src/io.c \
		      src/ustring.c \
		      src/unet.c \
		      src/ufile.c \
		      src/utable.c \
		      src/sqlite3.c

libccutils_la_LIBADD = -ldl

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/include

SUBDIRS = test

