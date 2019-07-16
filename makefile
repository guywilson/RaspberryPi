###############################################################################
#                                                                             #
# MAKEFILE for RaspberryPi Control Library                                    #
#                                                                             #
# � Guy Wilson 2016                                                           #
#                                                                             #
###############################################################################

# Source directory
SOURCE=src

# Build output directory
BUILD=build

# What is our target
TARGET=test

LIB=librpi.a

# C compiler
CPP=g++

# Linker
LINKER=g++

LIBTOOL=ar

# C compiler flags (Release)
CPPFLAGS=-c -fpermissive -Wall -std=c++11 

# Object files (in linker ',' seperated format)
OBJFILES=$(BUILD)/pifactory.o $(BUILD)/rasppi.o $(BUILD)/swtimer.o $(BUILD)/peripheral.o $(BUILD)/gpio.o $(BUILD)/clock.o $(BUILD)/pwm.o $(BUILD)/spi.o $(BUILD)/exception.o

# Target
all: $(TARGET)

# Compile C source files
#
$(BUILD)/test.o: $(SOURCE)/test.cpp $(SOURCE)/rasppi.h $(SOURCE)/gpio.h $(SOURCE)/types.h $(SOURCE)/swtimer.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/test.o $(SOURCE)/test.cpp

$(BUILD)/pifactory.o: $(SOURCE)/pifactory.cpp $(SOURCE)/pifactory.h $(SOURCE)/rasppi.h $(SOURCE)/exception.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/pifactory.o $(SOURCE)/pifactory.cpp

$(BUILD)/rasppi.o: $(SOURCE)/rasppi.cpp $(SOURCE)/rasppi.h $(SOURCE)/gpio.h $(SOURCE)/types.h $(SOURCE)/exception.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/rasppi.o $(SOURCE)/rasppi.cpp

$(BUILD)/swtimer.o: $(SOURCE)/swtimer.cpp $(SOURCE)/swtimer.h $(SOURCE)/exception.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/swtimer.o $(SOURCE)/swtimer.cpp

$(BUILD)/peripheral.o: $(SOURCE)/peripheral.cpp $(SOURCE)/peripheral.h $(SOURCE)/exception.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/peripheral.o $(SOURCE)/peripheral.cpp

$(BUILD)/gpio.o: $(SOURCE)/gpio.cpp $(SOURCE)/gpio.h $(SOURCE)/register.h $(SOURCE)/types.h $(SOURCE)/swtimer.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/gpio.o $(SOURCE)/gpio.cpp

$(BUILD)/clock.o: $(SOURCE)/clock.cpp $(SOURCE)/clock.h $(SOURCE)/register.h $(SOURCE)/types.h $(SOURCE)/exception.h $(SOURCE)/swtimer.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/clock.o $(SOURCE)/clock.cpp

$(BUILD)/pwm.o: $(SOURCE)/pwm.cpp $(SOURCE)/pwm.h $(SOURCE)/register.h $(SOURCE)/types.h $(SOURCE)/swtimer.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/pwm.o $(SOURCE)/pwm.cpp

$(BUILD)/spi.o: $(SOURCE)/spi.cpp $(SOURCE)/spi.h $(SOURCE)/register.h $(SOURCE)/types.h $(SOURCE)/swtimer.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/spi.o $(SOURCE)/spi.cpp

$(BUILD)/exception.o: $(SOURCE)/exception.cpp $(SOURCE)/exception.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/exception.o $(SOURCE)/exception.cpp

$(LIB): $(OBJFILES)
	$(LIBTOOL) rcs $(LIB) $(OBJFILES)

$(TARGET): $(OBJFILES) $(BUILD)/test.o $(LIB)
	$(LINKER) -L/usr/local/lib -L/usr/lib/x86_64-linux-gnu -L. -lstdc++ -lpthread -lrpi -o $(TARGET) $(BUILD)/test.o
