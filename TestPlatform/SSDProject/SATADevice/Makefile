
PACKAGE    = SATADevice
BUILDFLAGS = -g3 -fPIC  -Wall -W -pedantic -Wno-long-long -Wwrite-strings -Wpointer-arith 
CXX        = g++ 
CXXFLAGS = -fPIC -I$(SYSTEMC_INCLUDE)
LDFLAGS  = -Wl,--export-dynamic
COMMON_LIBS = -L$(SYSTEMC_LIBDIR) -lsystemc

PACKAGE_SOURCES = SATADevice.cpp \
									SATADevice_BDDI.cpp

SO = lib$(PACKAGE).so

$(SO) : $(PACKAGE_SOURCES:.cpp=.o)
	$(CXX) $(CXXFLAGS) -o $@ -shared $^ $(COMMON_LIBS) $(LDFLAGS) 

clean: 
	rm -f $(SO)
	rm -f *.o
