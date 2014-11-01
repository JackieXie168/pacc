AUTOMAKE_OPTIONS = gnu

#Build in these directories:
SUBDIRS = Util XML Math Threading Socket SVG

#Distribute these directories:
DIST_SUBDIRS = Util XML Math Threading Socket SVG

h_sources =  XML.hpp Util.hpp Math.hpp Socket.hpp SVG.hpp Threading.hpp

library_includedir=$(includedir)/PACC
library_include_HEADERS = $(h_sources)

EXTRA_DIST = $(h_sources) autogen.sh

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = paccutil.pc paccxml.pc paccmath.pc paccsocket.pc paccsvg.pc paccthreading.pc

