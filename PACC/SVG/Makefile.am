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

lib_LTLIBRARIES = libpacc-svg.la                            
libpacc_svg_la_LIBADD = ../Util/libpacc-util.la ../XML/libpacc-xml.la
libpacc_svg_la_SOURCES = $(h_sources) $(cc_sources)
libpacc_svg_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION)

