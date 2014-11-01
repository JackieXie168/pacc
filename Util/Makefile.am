h_sources = Assert.hpp \
            Date.hpp \
            MTRand.hpp \
            NumberGenerator.hpp \
            Randomizer.hpp \
            RandomPermutation.hpp \
            SignalHandler.hpp \
            StringFunc.hpp \
            Timer.hpp \
            Tokenizer.hpp

cc_sources =    Date.cpp \
                NumberGenerator.cpp \
                Randomizer.cpp \
                SignalHandler.cpp \
                Timer.cpp \
                Tokenizer.cpp
                
library_includedir = $(includedir)/PACC/Util
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

lib_LTLIBRARIES = libpaccutil.la
libpaccutil_la_SOURCES = $(h_sources) $(cc_sources)
libpaccutil_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION)
