h_sources = Canvas.hpp \
            Document.hpp \
            Frame.hpp \
            Group.hpp \
            Path.hpp \
            Primitives.hpp \
            Styles.hpp \
            Transforms.hpp \
            Types.hpp

cc_sources = Canvas.cpp \
             Document.cpp \
             Styles.cpp \
             Types.cpp
                
library_includedir = $(includedir)/PACC/SVG
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

noinst_LTLIBRARIES = libpacc-svg.la                            
libpacc_svg_la_SOURCES = $(h_sources) $(cc_sources)

