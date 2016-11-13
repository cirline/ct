lib_LTLIBRARIES = libccutils.la
libccutils_la_SOURCES = src/log.c \
			src/io.c \
			src/net.c \
			src/xml.c \
		      src/ustring.c \
		      src/ufile.c \
		      src/utable.c

libccutils_la_LIBADD = -ldl
libccutils_la_LIBADD += $(XML_LIBS)
libccutils_la_LIBADD	+= $(LIBCURL)

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CPPFLAGS += $(XML_CFLAGS)
# AM_CPPFLAGS	+= $(LIBCURL_CPPFLAGS)

SUBDIRS = test

