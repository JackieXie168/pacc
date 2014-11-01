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

INCLUDES = -I$(top_srcdir)

noinst_LTLIBRARIES = libpacc-socket.la                            
libpacc_socket_la_SOURCES = $(h_sources) $(cc_sources)


