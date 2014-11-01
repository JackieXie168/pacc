h_sources = Canvas.hpp \
            Frame.hpp \
            Group.hpp \
            Path.hpp \
            Primitives.hpp \
            Styles.hpp \
            Transforms.hpp \
            Types.hpp

cc_sources =    Canvas.cpp \
                Styles.cpp \
                Types.cpp
                
library_includedir = $(includedir)/PACC/SVG
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

lib_LTLIBRARIES = libpaccsvg.la                            
libpaccsvg_la_LIBADD = ../Util/libpaccutil.la ../XML/libpaccxml.la
libpaccsvg_la_SOURCES = $(h_sources) $(cc_sources)
libpaccsvg_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION)

