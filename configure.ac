#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)

dnl --------------------------------
dnl Specify INIT and CONFIG options
dnl --------------------------------
AC_INIT(efscape-gis, 1.0.0)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE
#AM_INIT_AUTOMAKE([subdir-objects])
LT_INIT
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/efscape/gis/GeoDataSource.hpp])
#AC_CONFIG_HEADER([config.h])

dnl -----------------------------------------------
dnl Adds configure options
dnl -----------------------------------------------

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_LIBTOOL_DLOPEN
AM_PROG_LIBTOOL
AM_SANITY_CHECK

dnl -----------------------------------------------
dnl Checks for libraries
dnl -----------------------------------------------

PKG_CHECK_MODULES(DEPS,
	liblog4cxx > 0.1)

BOOST_REQUIRE([1.60])

# Checks for libraries.
AC_CHECK_LIB([dl], [dlopen])
AC_CHECK_LIB([netcdf], [nc_open])

BOOST_SERIALIZATION()
BOOST_FILESYSTEM()
BOOST_SYSTEM()
BOOST_PROGRAM_OPTIONS()
BOOST_DATE_TIME()
BOOST_REGEX()

dnl ---------------------------------------------------------------------------
dnl Check for gdal (borrowed from grass 6.X.X.)
dnl ---------------------------------------------------------------------------
AC_PATH_PROG(GDAL_CONFIG, gdal-config, no)
if test "$GDAL_CONFIG" = "no" ; then
  AC_MSG_ERROR([*** couldn't find gdal-config])
fi

GDAL_LIBS=
GDAL_CFLAGS=
USE_GDAL=
USE_OGR=

if test "$GDAL_CONFIG" != "" ; then
  GDAL_LIBS=`"$GDAL_CONFIG" --libs`
  GDAL_DEP_LIBS=`"$GDAL_CONFIG" --dep-libs`
  GDAL_CFLAGS=`"$GDAL_CONFIG" --cflags`
  USE_GDAL=1
  if test `"$GDAL_CONFIG" --ogr-enabled` = "yes" ; then
    AC_DEFINE(HAVE_OGR)
    USE_OGR=1
  fi
fi

ac_save_libs="$LIBS"
LIBS="$GDAL_LIBS $LIBS"
AC_CHECK_FUNC(GDALOpen,[],[
LIBS="$GDAL_DEP_LIBS $LIBS"
AC_CHECK_FUNC(GDALOpen,[GDAL_LIBS="$GDAL_LIBS $GDAL_DEP_LIBS"],[
    AC_MSG_ERROR([*** Unable to locate GDAL library.])
])
])
LIBS=$ac_save_libs

AC_SUBST(GDAL_LIBS)
AC_SUBST(GDAL_CFLAGS)
AC_SUBST(USE_GDAL)
AC_SUBST(USE_OGR)

dnl ---------------------------------------------------------------------------
dnl Set geos library options
dnl ---------------------------------------------------------------------------
AC_PATH_PROG(GEOS_CONFIG, geos-config, no)
if test "$GEOS_CONFIG" = "no" ; then
  AC_MSG_ERROR([*** couldn't find geos-config])
fi

GEOS_CCLIBS=`"$GEOS_CONFIG" --cclibs`
GEOS_CFLAGS=`"$GEOS_CONFIG" --cflags`

AC_SUBST(GEOS_CCLIBS)
AC_SUBST(GEOS_CFLAGS)

dnl ---------------------------------------------------------------------------
dnl Set netcdf_c++ library options (to be edited manually)
dnl ---------------------------------------------------------------------------
NETCDF_CXX_LIBS=`nc-config --libs`" -lnetcdf_c++"
NETCDF_CXX_CFLAGS=`nc-config --cflags`

AC_SUBST(NETCDF_CXX_LIBS)
AC_SUBST(NETCDF_CXX_CFLAGS)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_TYPE_UINT32_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_HEADER_STDC
AC_CHECK_FUNCS([floor rint sqrt])

AC_CONFIG_FILES([Makefile
		 pkgconfig/efscape-gis.pc
		 src/Makefile
		 src/efscape/Makefile
		 src/efscape/gis/Makefile
		 src/efscape/gis/TestCode/Makefile])
AC_OUTPUT
