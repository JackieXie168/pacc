h_sources = Matrix.hpp \
            QRandSequencer.hpp \
            Vector.hpp

cc_sources =    Matrix.cpp \
                QRandSequencer.cpp \
                Vector.cpp
                
library_includedir = $(includedir)/PACC/Math
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

lib_LTLIBRARIES = libpacc-math.la
libpacc_math_la_LIBADD = ../Util/libpacc-util.la ../XML/libpacc-xml.la
libpacc_math_la_SOURCES = $(h_sources) $(cc_sources)
libpacc_math_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION)

