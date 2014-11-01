AUTOMAKE_OPTIONS = gnu

#Build temp static libraries in these directories:
SUBDIRS = Math Socket SVG Threading Util XML

#Distribute these directories:
DIST_SUBDIRS = Math Socket SVG Threading Util XML

h_sources =  Math.hpp Socket.hpp SVG.hpp Threading.hpp Util.hpp XML.hpp

library_includedir = $(includedir)/PACC
library_include_HEADERS = $(h_sources)

#Buils one static library and one dynamic library using the temp static libraries
lib_LTLIBRARIES = libpacc.la
libpacc_la_LIBADD = Threading/libpacc-threading.la Socket/libpacc-socket.la SVG/libpacc-svg.la Math/libpacc-math.la XML/libpacc-xml.la Util/libpacc-util.la
libpacc_la_SOURCES = $(h_sources)
libpacc_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION) $(PACC_ZLIB_LDFLAGS) -lpthread
