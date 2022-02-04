
# Makefile for the libGE intertwined spirals example using the GALib
# evolutionary library.
# If you are using this Makefile as a starting point for another
# libGE application, you may wish to check the settings below.


# General options.
OBJECTS           = GAmode.o
#HEADERS           =

# General compiler options.
CC                = gcc
CXX               = g++
CFLAGS           += -Wall -O2
CXXFLAGS         += -Wall -O2
#LDFLAGS          +=

# GALib options.
#CXXFLAGS         += -IGALIB_INCLUDE_DIR
#LDFLAGS          += -LGALIB_LIB_DIR
LIBS             += -lga

# libGE options.
#CXXFLAGS         += -ILIBGE_INCLUDE_DIR
#LDFLAGS          += -LLIBGE_LIB_DIR
#LIBS             += -lGE


default:all

all:    GAmode

GAmode:  $(OBJECTS) $(EVAL_OBJECT) $(HEADERS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

clean:
	rm -f *~ *.o a.out cc-output.dat stats\
	GAmode

# ---

GAmode.o: main.cpp #AutomodeAllele.h AutomodeGenome.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<
