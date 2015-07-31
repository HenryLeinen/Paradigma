# the compiler
CC  = gcc
CPP = g++

# compiler flags
# -Wall	turns on most, but not all, compiler warnings
# -g 	adds debuging information to the executable file
CFLAGS += -O2

# define the include directories
INC += -I./inc/

# define any libraries which are reguired
LIBS = -lstdc++ -lm -lwiringPi -lcurl-gnutls
LDIR = ./obj

# define the sources
CSRCS =
XSRCS = heizung.cpp ParadigmaData.cpp ParadigmaDateTime.cpp ParadigmaTemperature.cpp

#define the C object files
_COBJS = $(CSRCS:.c=.o)
COBJS = $(patsubst %,$(LDIR)/%,$(_COBJS))
_XOBJS = $(XSRCS:.cpp=.o)
XOBJS = $(patsubst %,$(LDIR)/%,$(_XOBJS))

#define the main executable file
MAIN = heizungd

# the following part of the makefile is generic

all:	clean $(MAIN)
	@echo Simple compiler named mycc has been compiled

$(MAIN): $(_XOBJS) $(_COBJS)
	@echo Linking;
	$(CC) -o $(MAIN) $(COBJS) $(XOBJS) $(LIBS)

%.o: ./src/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $(LDIR)/$@

%.o: ./src/%.cpp
	$(CPP) $(CFLAGS) $(INC) -c $< -o $(LDIR)/$@

.PHONY: depend clean

clean:
	$(RM) $(LDIR)/*.o *~ $(MAIN)

depend: ($CSRCS) ($XSRCS)
	makedepend $^


# DO NOT DELETE THIS LINE -- make depend needs it
