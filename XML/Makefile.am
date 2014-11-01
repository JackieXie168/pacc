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

lib_LTLIBRARIES = libpaccxml.la
libpaccxml_la_LIBADD = ../Util/libpaccutil.la
libpaccxml_la_SOURCES = $(h_sources) $(cc_sources)
libpaccxml_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION)
