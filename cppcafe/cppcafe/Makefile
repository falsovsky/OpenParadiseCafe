CXX=g++
CXXFLAGS=-Wall -pedantic-errors -g

SDLLIBS=`sdl-config --cflags --libs`
SDLINCLUDES=-I/usr/local/include

SOURCES=Sprite.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cppcafe

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(SDLLIBS) -o $@ $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(SDLLIBS) $(SDLINCLUDES) -c -o $@ $<
