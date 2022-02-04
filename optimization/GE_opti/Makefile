
# Makefile for the libGE intertwined spirals example using the GALib
# evolutionary library.
# If you are using this Makefile as a starting point for another
# libGE application, you may wish to check the settings below.


# General options.
OBJECTS           = main.o initfunc.o GEListGenome.o
#HEADERS           =

# General compiler options.
#CC                = gcc
#CXX               = g++
CFLAGS           += -Wall -O2
CXXFLAGS         += -Wall -O2
#LDFLAGS          +=

# GALib options.
#CXXFLAGS         += -IGALIB_INCLUDE_DIR
#LDFLAGS          += -LGALIB_LIB_DIR
LIBS             += -lga

# libGE options.
CXXFLAGS         += -ILIBGE_INCLUDE_DIR
LDFLAGS          += -LLIBGE_LIB_DIR
LIBS             += -lGE


# ---

# GEGCC options.
GEGCC_OBJECTS     = $(OBJECTS) automode-gcc.o
GEGCC_HEADERS     = $(HEADERS) $(GEGCC_EVAL_HDR)
GEGCC_CXXFLAGS    = $(CXXFLAGS) 
GEGCC_LDFLAGS     = $(LDFLAGS)
GEGCC_LIBS        = $(LIBS)

# ---


default:all

all:    GEGCC

GEGCC:  $(GEGCC_OBJECTS) $(GEGCC_EVAL_OBJECT) $(GEGCC_HEADERS)
	$(CXX) $(GEGCC_LDFLAGS) -o $@ $(GEGCC_OBJECTS) $(GEGCC_LIBS)

clean:
	rm -f *~ *.o a.out cc-output.dat stats\
	GEGCC GETCC GESLANG GELIBTCC GELUA

# ---


# GEGCC dependencies.

automode-gcc.o: automode-gcc.cpp
	$(CXX) -c $(GEGCC_CXXFLAGS) -o $@ $<
