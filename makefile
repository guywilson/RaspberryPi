###############################################################################
#                                                                             #
# MAKEFILE for Raspberry Pi Library                                           #
#                                                                             #
# (c) Guy Wilson 2021                                                         #
#                                                                             #
###############################################################################

# Directories
SOURCE = src
BUILD = build
DEP = dep

# What is our target
TARGET = librpi.a
GPIOC_LIB = libgpioc.a
LIB_TEST = test
GPIOC_TEST = gpioc_test

# Tools
VBUILD = vbuild
CPP = g++
C = gcc
LIBMGR = ar
LINKER = g++

# precompile step
PRECOMPILE = @ mkdir -p $(BUILD) $(DEP)
# postcompile step
POSTCOMPILE = @ mv -f $(DEP)/$*.Td $(DEP)/$*.d

CPPFLAGS = -c -O2 -Wall -pedantic -fPIC -std=c++11 -I/opt/vc/include
CFLAGS = -c -O2 -Wall -pedantic -fPIC
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP)/$*.Td
LIBFLAGS = rcs

# Libraries
STDLIBS = -lstdc++ -lpthread -lbcm_host

COMPILE.cpp = $(CPP) $(CPPFLAGS) $(DEPFLAGS) -o $@
COMPILE.c = $(C) $(CFLAGS) $(DEPFLAGS) -o $@
LIB.o = $(LIBMGR) $(LIBFLAGS) $@
LINK.o = $(LINKER) $(STDLIBS) -L/opt/vc/lib -o $@

CSRCFILES = $(wildcard $(SOURCE)/*.c)
CPPSRCFILES = $(wildcard $(SOURCE)/*.cpp)
OBJFILES := $(patsubst $(SOURCE)/%.c, $(BUILD)/%.o, $(CSRCFILES)) $(patsubst $(SOURCE)/%.cpp, $(BUILD)/%.o, $(CPPSRCFILES))
OBJFILES := $(filter-out $(BUILD)/test.o, $(OBJFILES))
OBJFILES := $(filter-out $(BUILD)/gpioc_test.o, $(OBJFILES))
DEPFILES = $(patsubst $(SOURCE)/%.c, $(DEP)/%.d, $(CSRCFILES)) $(patsubst $(SOURCE)/%.cpp, $(DEP)/%.d, $(CPPSRCFILES))

all: $(TARGET) $(GPIOC_LIB) $(LIB_TEST) $(GPIOC_TEST)

$(TARGET): $(OBJFILES)
	$(LIB.o) $^

$(GPIOC_LIB): $(BUILD)/gpioc.o
	$(LIB.o) $^

$(LIB_TEST): $(TARGET) $(BUILD)/test.o
	$(LINK.o) $^ $(TARGET)

$(GPIOC_TEST): $(GPIOC_LIB) $(BUILD)/gpioc_test.o
	$(LINK.o) $^ $(GPIOC_LIB)

$(BUILD)/%.o: $(SOURCE)/%.c
$(BUILD)/%.o: $(SOURCE)/%.c $(DEP)/%.d
	$(PRECOMPILE)
	$(COMPILE.c) $<
	$(POSTCOMPILE)

$(BUILD)/%.o: $(SOURCE)/%.cpp
$(BUILD)/%.o: $(SOURCE)/%.cpp $(DEP)/%.d
	$(PRECOMPILE)
	$(COMPILE.cpp) $<
	$(POSTCOMPILE)

.PRECIOUS = $(DEP)/%.d
$(DEP)/%.d: ;

-include $(DEPFILES)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin

clean:
	rm -r $(BUILD)
	rm -r $(DEP)
	rm $(TARGET)
