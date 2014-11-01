h_sources = Attribute.hpp \
            Document.hpp \
            Finder.hpp \
            Iterator.hpp \
            Node.hpp \
            Streamer.hpp

cc_sources =    Document.cpp \
                Finder.cpp \
                Node.cpp \
                Streamer.cpp
                
library_includedir = $(includedir)/PACC/XML
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

noinst_LTLIBRARIES = libpacc-xml.la
libpacc_xml_la_SOURCES = $(h_sources) $(cc_sources)

