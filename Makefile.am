AUTOMAKE_OPTIONS = gnu

#Build in these directories:
SUBDIRS = Util XML Math Threading Socket 

#Distribute these directories:
DIST_SUBDIRS = Util XML Math Threading Socket

h_sources =  XML.hpp Util.hpp Math.hpp Socket.hpp Threading.hpp

library_includedir=$(includedir)/PACC
library_include_HEADERS = $(h_sources)

EXTRA_DIST = $(h_sources)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = paccutil.pc paccxml.pc paccmath.pc paccsocket.pc paccthreading.pc

