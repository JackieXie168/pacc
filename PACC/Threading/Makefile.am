h_sources = Condition.hpp \
            Exception.hpp \
            Mutex.hpp \
            Semaphore.hpp \
            Task.hpp \
            Thread.hpp \
            ThreadPool.hpp \
            TLS.hpp

cc_sources =    Condition.cpp \
                Exception.cpp \
                Mutex.cpp \
                Semaphore.cpp \
                Thread.cpp \
                ThreadPool.cpp \
                TLS.cpp
                
library_includedir = $(includedir)/PACC/Threading
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

noinst_LTLIBRARIES = libpacc-threading.la
libpacc_threading_la_SOURCES = $(h_sources) $(cc_sources)


