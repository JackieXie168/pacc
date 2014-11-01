
dnl --------------------------------
dnl Initialization macros.
dnl --------------------------------

AC_INIT(configure.ac)
AM_CONFIG_HEADER(config.h)

dnl -----------------------------------------------
dnl Package name and version number (user defined)
dnl -----------------------------------------------

GENERIC_LIBRARY_NAME=pacc

GENERIC_MAJOR_VERSION=1
GENERIC_MINOR_VERSION=3
GENERIC_MICRO_VERSION=6
GENERIC_API_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_API_VERSION)

dnl Shared library versioning
GENERIC_LIBRARY_VERSION=4:0:0
dnl                     | | |
dnl              +------+ | +---+
dnl              |        |     |
dnl           current:revision:age
dnl              |        |     |
dnl              |        |     +- increment if interfaces have been added
dnl              |        |        set to zero if interfaces have been removed
dnl              |        |        or changed
dnl              |        +- increment if source code has changed
dnl              |           set to zero if current is incremented
dnl              +- increment if interfaces have been added, removed or changed

dnl --------------------------------
dnl Package name and version number substitution
dnl --------------------------------

AC_SUBST(GENERIC_LIBRARY_VERSION)

PACKAGE=$GENERIC_LIBRARY_NAME
AC_SUBST(GENERIC_LIBRARY_NAME)

GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION
GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION

AM_INIT_AUTOMAKE($PACKAGE, $VERSION, no-define)

dnl ----------------------------------
dnl Compilation options
dnl ----------------------------------

debug_default="no"
AC_ARG_ENABLE(debug, [  --enable-debug [=no/yes] Turning on debugging [default=no]],, enable_debug=$debug_default)
if test "x$enable_debug" = "xyes"; then
   CXXFLAGS="$CXXFLAGS -O0 -g3"
else
   CXXFLAGS="$CXXFLAGS -O2 -g0 -DPACC_NDEBUG"
fi

dnl -----------------------------------------------
dnl Checks for programs.
dnl -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL
AM_SANITY_CHECK
AC_LANG_CPLUSPLUS

dnl ----------------------------------
dnl Check for zlib
dnl ----------------------------------

AC_CHECK_HEADER(zlib.h)
if test "x$ac_cv_header_zlib_h" != "xno"; then
    CXXFLAGS="${CXXFLAGS} -DPACC_ZLIB"
    PACC_ZLIB_LDFLAGS=-lz    
else
    PACC_ZLIB_LDFLAGS=
fi
AC_SUBST(PACC_ZLIB_LDFLAGS)

dnl ----------------------------------
dnl Check for pthread (not activated for now...)
dnl ----------------------------------
dnl AC_CHECK_LIB(pthread, pthread_exit)
dnl if test "x$ac_cv_lib_pthread_pthread_exit" = "xyes"; then
dnl    CXXFLAGS="$CXXFLAGS -D_REENTRANT"
dnl    PACC_PTHREAD_LDFLAGS=-lpthread
dnl else
dnl    PACC_PTHREAD_LDFLAGS=
dnl fi
dnl AC_SUBST(PACC_PTHREAD_LDFLAGS)

dnl -----------------------------------------------
dnl Generates Makefile's, configuration files and scripts
dnl -----------------------------------------------

AC_OUTPUT(  Makefile \
            pacc.pc \
            PACC/Makefile \
            PACC/Math/Makefile \
            PACC/Socket/Makefile \
            PACC/SVG/Makefile \
            PACC/Threading/Makefile
            PACC/Util/Makefile \
            PACC/XML/Makefile \
)

