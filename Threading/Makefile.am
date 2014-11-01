h_sources = Condition.hpp \
            Exception.hpp \
            Mutex.hpp \
            Semaphore.hpp \
            Task.hpp \
            Thread.hpp \
            ThreadPool.hpp

cc_sources =    Condition.cpp \
                Exception.cpp \
                Mutex.cpp \
                Semaphore.cpp \
                Thread.cpp \
                ThreadPool.cpp
                
library_includedir = $(includedir)/PACC/Threading
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)/PACC -I$(top_srcdir)/PACC/Threading -I$(top_srcdir)/PACC/Util

lib_LTLIBRARIES = libpaccthreading.la
libpaccthreading_la_LIBADD = ../Util/libpaccutil.la 
libpaccthreading_la_SOURCES = $(h_sources) $(cc_sources)
libpaccthreading_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION) -lpthread

