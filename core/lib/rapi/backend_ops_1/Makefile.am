@SET_MAKE@

## create this library
noinst_LTLIBRARIES = librapiops1.la

## use these sources
librapiops1_la_SOURCES = database.c file_access.c file_management.c \
		invoke.c misc.c password.c registry.c
librapiops1_la_CFLAGS = -I$(top_srcdir)/lib/utils \
			-I$(top_builddir)/lib/utils \
			-I$(top_srcdir)/lib/rapi \
			-I$(top_srcdir)/lib/rapi/support

noinst_HEADERS = backend_ops_1.h
