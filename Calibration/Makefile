# the following lines pertain to setting up compiler options and libraries
OPTS = -Wall -g -Wno-deprecated
LIBS = -lGL -lglut -lm
ARCH := $(shell uname)
ifeq ($(ARCH), Linux)
else
	MACOSX_DEFINE = -DMACOSX -I/sw/include
	LIBS = -I/usr/common/include -I/usr/include/GL -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework GLUT -framework OpenGL -lGL -lm -lobjc -lstdc++
endif

BINARIES = bin/calibrate

# here are the usual make rules

all: $(BINARIES)

bin/calibrate: driver/calib.o driver/Image.o
	g++ $(OPTS) -o bin/calibrate driver/calib.o driver/Image.o $(LIBS)

driver/calib.o: driver/calib.cpp 
	g++ $(OPTS) $(MACOSX_DEFINE) -c driver/calib.cpp -o driver/calib.o

driver/Image.o: driver/Image.cpp driver/Image.h
	g++ $(OPTS) -c driver/Image.cpp -o driver/Image.o

clean:
	rm -f $(BINARIES) driver/*.o