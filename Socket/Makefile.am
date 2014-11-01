h_sources = Address.hpp \
            Cafe.hpp \
            ConnectedUDP.hpp \
            Exception.hpp \
            Port.hpp \
            TCP.hpp \
            TCPServer.hpp \
            UDP.hpp \
            UDPServer.hpp

cc_sources =    Address.cpp \
                Cafe.cpp \
                ConnectedUDP.cpp \
                Exception.cpp \
                Port.cpp \
                TCP.cpp \
                TCPServer.cpp \
                UDP.cpp \
                UDPServer.cpp
                
library_includedir = $(includedir)/PACC/Socket
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)/PACC -I$(top_srcdir)/PACC/Socket -I$(top_srcdir)/PACC/Threading -I$(top_srcdir)/PACC/Util

lib_LTLIBRARIES = libpaccsocket.la                            
libpaccsocket_la_LIBADD = ../Util/libpaccutil.la ../Threading/libpaccthreading.la
libpaccsocket_la_SOURCES = $(h_sources) $(cc_sources)
libpaccsocket_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION) $(PACC_ZLIB_LDFLAGS)

