# source directory for the efscape::gis::test programs

bin_PROGRAMS = efgistest

efgistest_SOURCES = TestCode.cc main.cc
testcode_sources = $(efgistest_SOURCES) TestCode.hh

efgistest_LDADD = $(top_srcdir)/src/efscape/gis/libefscape-gis.la

AM_CPPFLAGS = -I$(top_srcdir)/src
AM_CPPFLAGS += $(DEPS_CFLAGS)
AM_CPPFLAGS += $(BOOST_CPPFLAGS)
AM_CPPFLAGS += $(GEOS_CFLAGS)
AM_CPPFLAGS += $(NETCDF_CXX_CFLAGS)

LIBS = $(DEPS_LIBS)
LIBS += $(BOOST_DATE_TIME_LIBS)
LIBS += $(BOOST_SYSTEM_LIBS)

copyright:
	cp $(top_srcdir)/Copyright.doc $(top_srcdir)/Makefile.cr $(top_srcdir)/Sed.cr .
	make -f Makefile.cr NAME="${PACKAGE}" FILES="${testcode_sources}"
	rm -f Copyright.doc Makefile.cr Sed.cr
