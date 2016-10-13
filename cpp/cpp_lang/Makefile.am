# ACLOCAL_AMFLAGS cannot use top_scrdir etc.
ACLOCAL_AMFLAGS         = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libcpplang.la
libcpplang_la_SOURCES = src/vector.cpp \
			src/object.cpp

SUBDIRS	= test
