AUTOMAKE_OPTIONS = gnu

#Build in these directories:
SUBDIRS = Util Threading Socket XML Math SVG

#Distribute these directories:
DIST_SUBDIRS = Util Threading Socket XML Math SVG

h_sources =  Math.hpp Socket.hpp SVG.hpp Threading.hpp Util.hpp XML.hpp

library_includedir = $(includedir)/PACC
library_include_HEADERS = $(h_sources)

EXTRA_DIST = $(h_sources)
