AM_CXXFLAGS = @LIBRRA_CFLAGS@
AM_LDFLAGS = @LIBRRA_LIBS@

INCLUDES = -I$(top_srcdir)/libksynce -I$(top_srcdir)/includes \
	   $(KITCHENSYNC_INCLUDES) $(all_includes)

METASOURCES = AUTO
lib_LTLIBRARIES = librakikpimsync.la
librakikpimsync_la_LDFLAGS = -avoid-version -module $(all_libraries)
noinst_HEADERS = rakikpimsyncfactory.h rakikpimsync.h paireditordialog.h \
	paireditorwidget.h pimsyncmanager.h synceengine.h
librakikpimsync_la_SOURCES = rakikpimsyncfactory.cpp rakikpimsync.cpp \
	paireditorwidget.cpp pimsyncmanager.cpp paireditordialog.cpp synceengine.cpp

kde_services_DATA = rakikabcsync.desktop rakikcalsync.desktop \
	rakiktasksync.desktop
librakikpimsync_la_LIBADD = $(top_builddir)/libksynce/libksynce.la \
	$(LIB_MULTISYNK) $(LIB_KABC_FILE) $(LIB_KONNECTOR) $(LIB_KSYNC)
