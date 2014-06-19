#############################################################################
#
# Project Makefile
#
# Tom Verloop
# 
#############################################################################

# Specify the project name, this is also the main .cpp file to compile
PROJECT    := ./src/main

# Specify target chip or board or system
TARGET     := lpc1114fn28

# Specify the location of the bmptk library
BMPTK      := C:/bmptk

# Specify project files (other than $(PROJECT).cpp) (if any)
OBJECTS    := 
SOURCES    := 
HEADERS    := 
LIBS       := 

#Specify the nessesary folders which contain the libs
SEARCH	   := ../BMPTK_LIBS/src
# specify (extra) results you want to get (if any)
RESULTS    := 
# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
