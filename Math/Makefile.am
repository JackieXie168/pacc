h_sources = Matrix.hpp \
            Vector.hpp

cc_sources =    Matrix.cpp \
                Vector.cpp
                
library_includedir = $(includedir)/PACC/Math
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

lib_LTLIBRARIES = libpaccmath.la
libpaccmath_la_LIBADD = ../Util/libpaccutil.la ../XML/libpaccxml.la
libpaccmath_la_SOURCES = $(h_sources) $(cc_sources)
libpaccmath_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION)

