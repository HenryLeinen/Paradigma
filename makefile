# the compiler
CC  = gcc
CPP = g++

# compiler flags
# -Wall	turns on most, but not all, compiler warnings
# -g 	adds debuging information to the executable file
CFLAGS += -O2
CFLAGS += -DBIG_JOINS=1
#CFLAGS += -fno_strict_aliasing
CFLAGS += -g
#CFLAGS += -DDEBUG

# define the include directories
INC += -I./inc/
INC += -I/usr/include/mysql

# define any libraries which are reguired
LIBS = -lstdc++ -lm -lwiringPi -lcurl-gnutls
LIBS += -lpthread -lm -lz -lrt -ldl
LDIR = ./obj
LIBDIR = $(shell mysql_config --libs)

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

.PHONY: heizungd
$(MAIN): $(_XOBJS) $(_COBJS)
	@echo Linking;
	$(CC) -o $(MAIN) $(COBJS) $(XOBJS) $(LIBDIR) $(LIBS)

%.o: ./src/%.c makefile
	$(CC) $(CFLAGS) $(INC) -MMD -MP -c $< -o $(LDIR)/$@

%.o: ./src/%.cpp makefile
	$(CPP) $(CFLAGS) $(INC) -MMD -MP -c $< -o $(LDIR)/$@

.PHONY: depend clean

clean:
	$(RM) $(LDIR)/*.o *~ $(MAIN)

depend: ($CSRCS) ($XSRCS)
	makedepend $^


# DO NOT DELETE THIS LINE -- make depend needs it
