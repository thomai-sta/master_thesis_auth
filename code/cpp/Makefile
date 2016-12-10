SOURCES=main.cpp correlation.cpp demo.cpp imageRepresentation.cpp \
 experiments.cpp

CXX=g++
CXXFLAGS=-O3 -Wall -rdynamic

#~ INCLUDES=`pkg-config --cflags opencv`
INCLUDES=-I./inc/opencv -I./inc
LDFLAGS=-L/share/mug/lib -lfftw3\
 -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d \
 -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy \
 -lopencv_ml -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching \
 -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lm

OBJECTS=$(SOURCES:.cpp=.o)
HEADERS=$(SOURCES:.cpp=.h)

%.o : %.cpp
	$(CXX) -I. $(CXXFLAGS) $(INCLUDES) -c $< -o $@

thesis:$(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
all: thesis
clean:
	rm -f *.o thesis

deps/%.d: %.cpp
	@set -e; $(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) -I. $(OPTIONS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : Makefile ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

-include $(addprefix deps/,$(SOURCES:.cpp=.d))
